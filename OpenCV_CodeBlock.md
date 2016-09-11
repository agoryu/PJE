# Pour avoir OpenCV sur codeblock il faut:  

1. Installer OpenCV (http://doc.ubuntu-fr.org/opencv)
2. Etre sur un projet codeblock
3. Dans le menu **Project/Build Option** pour le projet complet (pas dans Debug ni Release), puis dans l'onglet **Search directories** et sous onglet **Compiler**, il faut ajouter /opt/opencv-2.4.10/include/opencv selon où est installé OpenCV
4. Toujours dans la fenetre **Build Option** dans l'onglet **Linker settings** et dans le TextArea **Other linker options**, il faut mettre \`pkg-config opencv --libs\` (avec les apostrophe)
5. Toujours dans la fenetre **Build Option**, Onglet **Compiler settings**, sous onglet **Other options**, il faut mettre \`pkg-config opencv --cflags\` (avec les apostrophes)

