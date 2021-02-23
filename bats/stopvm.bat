sc stop "VMwareHostd"
sc config "VMwareHostd" start= demand

sc stop "VMUSBArbService"
sc config "VMUSBArbService" start= demand

sc stop "VMware NAT Service"
sc config "VMware NAT Service" start= demand

sc stop "VMNetDHCP"
sc config "VMNetDHCP" start= demand

sc stop "VMAuthdService"
sc config "VMAuthdService" start= demand

sc stop "VMUSBArbService"
sc config "VMUSBArbService" start= demand

sc stop "vmms"
sc config "vmms" start= demand

sc stop "vmcompute"
sc config "vmcompute" start= demand

pause