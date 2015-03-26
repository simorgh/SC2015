GiVD - curs 2014-2015 – Pràctica 2
======================================
##Pràctica número 2: Afegir càmera interactiva en el billar

Tema: Implementació del pipeline gràfic: a partir de la definició de l'escena, realitzar el control de la càmera i
navegació 3D.

Objectiu: Visualització d'una escena 3D amb control de la càmera amb Qt i OpenGL utilitzant shaders.
Visualització del joc en tercera persona i en primer persona.

La pràctica té com objectiu afegir una càmera dinàmica controlada des de la interfície per a poder obtenir
diferents vistes de l’escena de la sala de billar. A més a més s’afegirà una segona càmera en primera
persona, per a controlar la direcció de tir de la bola blanca. A la pràctica 1 has construït i has visualitzat el
pla base i les boles des d'un punt de vista determinat per GL i les rotacions es feien directament sobre els
models. En la pràctica 2, es defineix el concepte càmera que permet explorar-los des de diferents punts de
vista, amb diferents factors de zoom i variant el centre de projecció (operació de panning). Així mateix, es
permetrà la visualització en primera persona des del punt de vista de la bola blanca per a poder controlar la
direcció de tir en el joc del billar. 

L’escena contindrà dues càmeres: la càmera general, que permetrà visualitzar el pla base i les boles i la
càmera en primera persona que controlarà el tir de la bola blanca en el joc del billar. La pràctica 2 es
composa de 4 parts:
− Modificació de la classe Càmera (proporcionada en el campus virtual de l’assignatura) 
per incloure el càlcul de les matrius model-view i projection, que permetin tant la visualització de l'escena
completa com parts d'ella, des de la GPU.
− Inclusió de dues càmeres en l’escena: la càmera general i la càmera en primera persona.
− Implementació dels mètodes a la classe Escena: initCamera, setAnglesCamera, setVRPCamera,
setWindowCamera, setDCamera que permetran la inicialització/modificació de les càmeres i
tornar a calcular les matrius model-view i projection de les càmeres, depenent de les modificacions
que es facin en el atributs de la càmera. Utilitzaran mètodes ja implementats de la classe càmera.
− Interacció i activació de les possibles modificacions de les càmeres controlades pel teclat i el ratolí.
