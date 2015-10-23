rem stop bluestack services
sc stop "BstHdAndroidSvc"
sc config "BstHdAndroidSvc" start= demand
sc stop "BstHdLogRotatorSvc"
sc config "BstHdLogRotatorSvc" start= demand
sc stop "BstHdUpdaterSvc"
sc config "BstHdUpdaterSvc" start= demand

sc stop KstHdAndroidSvc_K8
sc config "KstHdAndroidSvc_K8" start= demand
sc stop KstHdLogRotatorSvc_K8
sc config "KstHdLogRotatorSvc_K8" start= demand
sc stop KstHdUpdaterSvc_K8
sc config "KstHdUpdaterSvc_K8" start= demand
pause