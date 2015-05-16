SC - curs 2014-2015 – OpenMP
======================================
##Pràctica de OpenMP: Cercador d’imatges

### Introducció

L’objectiu d’aquesta pràctica és implementar una aplicació que permeti cercar imatges fent servir l’histograma per descriure-les. En particular, l’aplicació ha d’implementar les següents funcionalitats.

* Extracció de l’histograma de les imatges.
* Cerca d’imatges similars a partir d’una imatge exemple.
* Persistència en les dades extretes de la imatge.

###### User instructions

## Used Image set
*http://www.vision.caltech.edu/Image_Datasets/Caltech256/

```
# Persistence-Tree structure:

	['db']
	  |____ ['images']
  	  |	
  	  |____ ['histrograms']
  	  |
  	  |____ '.id' (hidden)

# Loadfile generation instruc. for a new set of images '256_ObjectCategories':

:pwd$ cd /absolute_path_to_db/256_ObjectCategories/
:256_ObjectCategories$ find -name *.jpg >> ./path_to_project_folder/filename.txt

# OSX requires '.' as it does not infer current dir
MacBook-Pro:256_ObjectCategories user$ find . -name *.jpg >> ../filename.txt
```
#### see 'example_list.txt' in order to make an idea

### Install instructions for OpenCV + QT
*Ubuntu: http://xrong.org/notes/install-Qt-OpenCV-Ubuntu
*Windows: http://stackoverflow.com/questions/14919186/qt-creator-5-0-1-with-opencv-2-3-4-on-windows
*Mac: https://drthitirat.wordpress.com/2014/09/21/install-opencv-2-4-9-and-qt-5-3-2-on-mac-osx-10-9-5-and-xcode-6-0-1/
