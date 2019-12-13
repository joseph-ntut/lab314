# **Use command to load rule file**
```js
sudo cp *.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```