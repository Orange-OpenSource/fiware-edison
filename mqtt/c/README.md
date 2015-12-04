* Access your Edison via SSH (your laptop should be in the same WIFI network your Edison is)
```
ssh root@your_edison_ip
```

* Configure your FIWARE credentials:
```
#define FIWARE_APIKEY "xxxxxxxxxxx"
#define CLIENTID "xxxxxxx"
```

* Run your programm :  
```
make && ./mqtt
``` 