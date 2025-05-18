#/bin/bash

sudo apt install samba
sudo adduser shareuser
sudo smbpasswd -a shareuser
sudo mkdir -p /srv/nas/share
sudo chown shareuser:shareuser /srv/nas/share
