PROBLEM?
========


░░░░▄▄▄▄▀▀▀▀▀▀▀▀▄▄▄▄▄▄  
░░░░█░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░▀▀▄  
░░░█░░░▒▒▒▒▒▒░░░░░░░░▒▒▒░░█  
░░█░░░░░░▄██▀▄▄░░░░░▄▄▄░░░█  
░▀▒▄▄▄▒░█▀▀▀▀▄▄█░░░██▄▄█░░░█  
█▒█▒▄░▀▄▄▄▀░░░░░░░░█░░░▒▒▒▒▒█  
█▒█░█▀▄▄░░░░░█▀░░░░▀▄░░▄▀▀▀▄▒█  
░█▀▄░█▄░█▀▄▄░▀░▀▀░▄▄▀░░░░█░░█  
░░█░░▀▄▀█▄▄░█▀▀▀▄▄▄▄▀▀█▀██░█  
░░░█░░██░░▀█▄▄▄█▄▄█▄████░█  
░░░░█░░░▀▀▄░█░░░█░███████░█  
░░░░░▀▄░░░▀▀▄▄▄█▄█▄█▄█▄▀░░█  
░░░░░░░▀▄▄░▒▒▒▒░░░░░░░░░░█  
░░░░░░░░░░▀▀▄▄░▒▒▒▒▒▒▒▒▒▒░█  
░░░░░░░░░░░░░░▀▄▄▄▄▄░░░░░█  



Le fichier tp4.cbp est comme un fichier xml, on peut l'éditer!
Il faut l'ouvrir et modifier dans les balises **Compiler** et **Linker** 
les balises **Add** et changer la valeur de leurs attributs.

 * * *

**Exemple:**

    <Compiler>
	    <Add option="-pedantic" />
	    <Add option="-std=c++11" />
	    <Add option="-Wall" />
	    <Add option="-ansi" />
	    <Add option="-fexceptions" />
	    <Add option="`pkg-config opencv --cflags`" />
	    <Add directory="/opt/opencv-2.4.10/include/opencv" />
    </Compiler>
	<Linker>
		<Add option="`pkg-config opencv --libs`" />
	</Linker>
