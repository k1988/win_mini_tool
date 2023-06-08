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

@rem VMware Bridge Protocal
sc stop "vmnetbridge"
sc config "vmnetbridge" start= demand

@rem VMware Virtual Ethernet Userif
sc stop "vmnetuserif"
sc config "vmnetuserif" start= demand
 
@rem VMware Input Filter and Injection Driver (vmkbd)
@rem c:\windows\system32\drivers\vmkbd.sys
sc stop "vmkbd3"
sc config "vmkbd3" start= demand

sc stop "vmcompute"
sc config "vmcompute" start= demand

pause