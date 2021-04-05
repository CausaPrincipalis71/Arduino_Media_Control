if [ $USER != "root" ]
then
echo "Please, run this as super user"
exit
fi

directory = $(pwd)

cmake ./
make 

mv ./Arduino_Media_Control /bin/Arduino_Media_Control
cd /bin

echo -e "#!/bin/bash\n$(pwd)/Arduino_Media_control" > Arduino_Media_Control.sh

chmod +x Arduino_Media_Control.sh

cd /etc/systemd/system 

echo -e "[Unit]\nDescription = Mouse Control\n\n[Service]\nRemainAfterExit=true\nExecStart=/bin/Arduino_Media_Control.sh\nType=oneshot\n\n[Install]\nWantedBy=multi-user.target\n" > Arduino_Media_Control.service

systemctl enable Arduino_Media_Control.service

echo "*****INSTALLATION DONE*****"
