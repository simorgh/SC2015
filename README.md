SC - curs 2014-2015 – OpenCL
======================================
##Pràctica de OpenCL: Multiplicació de Matrius

#Definició de la pràctica

La multiplicació de matrius es una operació matemàtica bàsica, que conté força paral·lelisme de dades, però també certs problemes de localitat de dades. En aquesta pràctica se os demana que implementeu el codi en OpenCL per a la multiplicació de dues matrius A i B, i retorneu una matriu C amb el resultat de la multiplicació.

Per a simplificar, A i B tindran la mateixa mida per a les dimensions “x” i “y”. Junt amb aquest enunciat, teniu una plantilla de codi on trobareu la solució en C sense paral·lelitzar, per comprovar que els resultats en OpenCL són  correctes. Podeu utilitzar aquest codi per començar, tot i que haureu  de fer forces canvis. El codi de Host el podeu implementar amb  SimpleOpenCL. El codi de Devic e, és a dir, el Kernel, l’heu de fer amb OpenCL.
