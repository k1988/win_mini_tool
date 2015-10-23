sc stop "Apple Mobile Device"
sc config "Apple Mobile Device" start= demand
sc stop "iPod Service"
sc config "iPod Service" start= demand
sc stop "Bonjour Service"
sc config "Bonjour Service" start= demand
