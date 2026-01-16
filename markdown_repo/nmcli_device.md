# nmcli
Net work manager client

### For new connection
```bash
# dev == d, c == connection
nmcli device wifi list # check wifi list
nmcli device wifi connect "SSID" password "password" # connect command
```

### For saved connections
~~~bash
nmcli connection show # show connections already saved
nmcli connection up "name" # connect to...
nmcli connection down "name" # disconnect
~~~

### Ethnet
~~~bash
nmcli device status # check status
nmcli device connect eth0 # connect to ethnet
~~~
