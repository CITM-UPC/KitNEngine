Actualmente el motor sirve para poder visualizar modelos 3d con texturas.

https://github.com/CITM-UPC/KitNEngine
Roger Puchol y Eric Palomares, https://github.com/Ropuce , https://github.com/errico6

Funciona de la siguiente forma: Mover la cámara con las teclas a w s d + clic derecho
Alt + clic izquierdo para cámara orbital. Dicha cámara orbita sobre un punto a una distancia determinada de la cámara
Rueda del ratón para zoom
F para centrar sobre (0,0,0)
Se puede usar la Ui superior para salir y acceder al GitHub, opción para "crear" formas primitivas,
y ventanas para la jerarquía, inspector, configuración, y consola
Se pueden arrastrar ficheros .fbx para modelos
También son compatibles los ficheros .png, .jpg y .dds para texturas


Cosas nuevas en la entrega 2:
- La consola ya funciona, y se puede limpiar y limitar la cantidad de texto máximo
- Ya hay una configuración con multiples opciones para apagar o activar y modificar valores
- Se muestran los AABB del objeto seleccionado
- El stencil esta a medias, pero se puede ver que en parte funciona
- Importación y gestión de recursos parcialmente implementados
- Inspector funcional, permite ver y modificar el transform del gameobject y sus componentes
- Como modelo inicial se carga el modelo proporcionado en la pagina de la entrega
- En su mayoría el código del juego está separado del editor, excepto por el código que muestra el contenido del inspector
- Jerarquía de objetos, transforms para cada GameObject y gestión de componentes implementados
- MeshRenderer como componente de GameObject y usando el transform de este