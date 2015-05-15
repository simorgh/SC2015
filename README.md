SC - curs 2014-2015 – OpenMP
======================================
##Pràctica de OpenMP: Cercador d’imatges

###Introducció

L’objectiu d’aquesta pràctica és implementar una aplicació que permeti cercar imatges fent servir l’histograma per descriure-les. En particular, l’aplicació ha d’implementar les següents funcionalitats.

* Extracció de l’histograma de les imatges.
* Cerca d’imatges similars a partir d’una imatge exemple.
* Persistència en les dades extretes de la imatge.

###### User instructions
```
# Persistence-Tree structure:

	['db']
	  |____ ['images']
  	  |	
  	  |____ ['histrograms']
  	  |
  	  |____ '.id' (hidden)

# Loadfile generation instruc. for a new set of images 'image_db':

:pwd$ cd /absolute_path_to_db/image_db/
:absolute_path_to_db$ find -name *.jpg >> ./path_to_project_folder/filename.txt

```