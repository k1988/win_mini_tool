sc stop "VMware NAT Service"
sc config "VMware NAT Service" start= demand

sc stop "VMNetDHCP"
sc config "VMNetDHCP" start= demand

sc stop "VMAuthdService"
sc config "VMAuthdService" start= demand

sc stop "VMUSBArbService"
sc config "VMUSBArbService" start= demand

sc stop "VMAuthdService"
sc config "VMAuthdService" start= demand
pause