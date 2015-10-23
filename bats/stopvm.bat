sc stop "VMware NAT Service"
sc config "VMware NAT Service" start= demand
sc stop "VMNetDHCP"
sc config "VMNetDHCP" start= demand
pause