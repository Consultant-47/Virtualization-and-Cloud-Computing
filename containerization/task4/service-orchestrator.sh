#!/bin/bash

# Complete this script to deploy external-service and counter-service in two separate containers
# You will be using the conductor tool that you completed in task 3.

# Creating link to the tool within this directory
ln -s ../task3/conductor.sh conductor.sh
ln -s ../task3/setup.sh setup.sh

# use the above scripts to accomplish the following actions -

# Logical actions to do:
# 1. Build images for the containers
./conductor.sh build es esfile
./conductor.sh build cs csfile

# 2. Run two containers say es-cont and cs-cont which should run in background. Tip: to keep the container running
#    in background you should use a init program that will not interact with the terminal and will not
#    exit. e.g. sleep infinity, tail -f /dev/null
./conductor.sh run es es-cont -- "sleep infinity" &
./conductor.sh run cs cs-cont -- "sleep infinity" &
sleep 2

# 3. Configure network such that:
#    3.a: es-cont is connected to the internet and es-cont has its port 8080 forwarded to port 3000 of the host
./conductor.sh addnetwork es-cont -i -e 8080-3000

#    3.b: cs-cont is connected to the internet and does not have any port exposed
./conductor.sh addnetwork cs-cont -i

#    3.c: peer network is setup between es-cont and cs-cont
./conductor.sh peer es-cont cs-cont

# 5. Get ip address of cs-cont. You should use script to get the ip address. 
#    You can use ip interface configuration within the host to get ip address of cs-cont or you can 
#    exec any command within cs-cont to get it's ip address
IP=$(ip a show cs-cont-outside | awk '/inet / {print $2}' | cut -d/ -f1 | awk -F. '{OFS="."; $4="2"; print}')
echo "IP Address of cs-cont-inside is: $IP"

# exit 0

# 6. Within cs-cont launch the counter service using exec [path to counter-service directory within cs-cont]/run.sh
# ./conductor.sh exec cs-cont -- "/cs/counter-service 8080 1 &"
# ./conductor.sh exec cs-cont -- "nohup /cs/counter-service 8080 1 > /dev/null 2>&1 &"
# ./conductor.sh exec cs-cont -- bash -c "nohup /cs/counter-service 8080 1 > /dev/null 2>&1 &"
# ./conductor.sh exec cs-cont -- "/bin/sh -c "/cs/counter-service 8080 1" &"
./conductor.sh exec cs-cont -- "/cs/counter-service 8080 1"

# 7. Within es-cont launch the external service using exec [path to external-service directory within es-cont]/run.sh
./conductor.sh exec es-cont -- "python3 /es/app.py "http://$IP:8080/" &"

# 8. Within your host system open/curl the url: http://localhost:3000 to verify output of the service
MYIP=$(hostname -I | awk '{print $1}')
curl "http://$MYIP:3000"

# 9. On any system which can ping the host system open/curl the url: `http://<host-ip>:3000` to verify
#    output of the service