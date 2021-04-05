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

echo -e "[Desktop Entry]\nName=Arduino_Media_Control\nNoDisplay=true\nType=Application\nTerminal=false\nExec=Arduino_Media_Control" > /etc/xdg/autostart/Arduino_Media_Control

echo "*****INSTALLATION DONE*****"
