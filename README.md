En este repositorio añado lo básico para las práticas del máster con algunas macros y las librerías necesarias para correr las cosas. Las carpetas ROOT y AnalysisROOT son para almacenar archivos .root que generemos. En el .zip tengo los datos de calibración y con fuentes de luz (que creo que no se harán en la práctica, pero por si acaso) tanto para RT como en LN2. Aun así, en la carpeta ROOT he dejado un archivo de ejemplo. En la carpeta lib están todas las librerías de las macros, mejor no manipular mucho esos archivos...

Macros y librerías necesarias para el análisis de los datos tomados con el ADC o el osciloscopio. Las macros que se presentan son:

  1.) FirstDataProcess.C: esta macro es la encargada de la lectura de los datos en binario (ADC) o en .txt (osciloscopio). En ella aplico un par de bucles para recorrer todos los OVs a los que se opera y las longitudes de onda de las fuentes de luz. Las funciones que se usan son ReadAndDumpBinary (leer ADC) y ReadAndDumpOscilloscope (leer osciloscopio), que se recogen al final de la librería FirstDataProcess.h. Esta librería contiene varias funciones que a su vez se usan en las dos nombradas anteriormente. 
  
  Básicamente, en ambos casos, lee la estructura de los archivos raw y los vuelca en un archivo con extensión .root con una estructura determinada (ver función Dump en FirstDataProcess.h) usando la clase TTree de ROOT. Esta es la estructura que necesitan el resto de macros para que funcionen correctamente.
  Los argumentos de ambas funciones son el path donde se encuentran los datos raw y el path donde queremos volcar los archivos .root. En este caso se puede trabajar en el directorio local y volcar los archivos generados en una carpeta llamada ROOT.

  2.) EventDisplay.C: en esta macro he juntado la visualización de los eventos y el histograma de carga. Se empieza cargando el archivo .root que se quiere analizar, los argumentos que se han de introducir están nombrados en la macro. Se vuelca a una clase llamada Run_t, sobre la que se aplicarán las funciones que queramos. Todas ellas (al igual que la definición de la clase) se recogen en la librería Run.h.
  
  Seguidamente se activan algunas funciones que son necesarias para que no se rompa todo (es más fácil dejarlas ahí que modificar el código para que no sean necesarias). Cuando se llega a la línea 28 de la macro ya se empiezan a aplicar funciones de visualización. Por ejemplo, las dos primeras es para ver los tiempos en los que se alcanzan los máximos de cada evento (nos podemos hacer una idea de en qué rangos de tiempo se encuentran las señales) y para ver un histograma de la STD del pedestal. Después he puesto algunos cortes de ejemplo que se pueden aplicar para tirar eventos (todos los posibles están recogidos en Cut.h, pero para el osciloscopio no funcionan todos porque todo esto está pensado para el ADC...).

  Ya en la línea 37 se usa una función para visualizar los eventos individualmente. Aparecen 4 líneas: dos verdes y dos rojas. Las dos verdes son los tiempos "range1" y "range2" que son los que se introducen cuando se carga la clase Run_t. Aquí se usará para establecer el tiempo de integración. Así podemos ver en cada evento qué parte va a integrar. Se puede jugar con estos rangos para ver cómo cambia el espectro de carga. La línea roja vertical es la que marca el límite hasta el que se calcula el pedestal, es decir, de 0 hasta esa línea son los bines que se emplean para sacar el nivel del pedestal. Y la línea roja horizontal es precisamente este valor del pedestal. Para estas medidas en concreto, como tenemos ventanas muy cortas, el tiempo del cálculo de pedestal es pequeño y por tanto muchas veces estará mal determinado, pero como son señales un poco "locas" no se puede hacer mucho más.

  Ya finalmente se representa el plot de carga intengrando entre range1 y range2, en unidades de V*ns para el osciloscopio. También he añadido una función que te realiza un ajuste al número de gaussianas que le digas entre los valores del eje x que le marques (todo esto en la propia terminal).

  3.) AverageWaveform.C: esta macro es la que se usa para sacar la waveform media de todos los eventos, o de aquellos que queden tras los cortes. En principio no creo que la vayan a usar pero por si es interesante la añado. Aunque sacar la media de estos eventos es un poco engañoso porque tenemos señal por todos lados y la anchura de la señal real se puede ver afectada. Esto es más útil para señales más intensas o que hagan trigger externo con láser. 


# Practica_master
# Practica_master
# Practica_master
