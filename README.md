[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-f059dc9a6f8d3a56e377f745f24479a46679e63a5d9fe6f495e02850cd0d8118.svg)](https://classroom.github.com/online_ide?assignment_repo_id=7421586&assignment_repo_type=AssignmentRepo)
# Find-My-Mates
## Índice
- [Objetivo principal](#Objetivo-principal)
- [Recorrido](#Recorrido)
- [Mapa](#Mapa)
- [Reglas y comentarios adicionales](#Reglas-y-comentarios-adicionales)
- [Instrucciones del árbitro](#Instrucciones-del-árbitro)
- [Instrucciones del CO](#Instrucciones-del-CO)
- [Hoja de puntuacion](#Hoja-de-puntuacion)
- [Penalizaciones por no asistir](#Penalizaciones-por-no-asistir)
- [Launch y Publisher del Monologo](#Launch-y-Publisher-del-Monologo)
- [Publicar puntos en el navegation](#Publicar-puntos-en-el-navegation)
- [Roslint](#Roslint)

## Objetivo principal
En esta prueba, el robot tendrá que entrar a la arena en busca de sus compañeros y notificar al arbitro el nombre
de estos e información extra de ellos.

## Recorrido
1. El robot empezará en el punto de inicio y tendrá que entrar dentro de la arena.
2. Dentro de la arena, tendrá que localizar a la primera persona.
3. Una vez localizada, tendrá que acercarse a ella.
4. El robot tendrá que preguntarle el nombre y obtener el color de camiseta y el objeto que lleva.
5. Después, el robot tendrá que ir al arbitro a reportarle la información obtenida o seguir buscando personas
y reportar todas al final.

## Mapa

![image](https://user-images.githubusercontent.com/90789825/167893945-866015df-cdeb-4221-b71f-f3fd2b7b0063.png)

## Reglas y comentarios adicionales
1. **Deus ex Machina:** Reducción de puntaje por solicitar asistencia humana por huésped.
   - Si una persona tiene que agitar el robot para ser encontrada. **Penalty:** -75pts.
   - Si una persona tiene que decirle al robot dónde está sentada/de pie. **Penalty:** -75pts.
   - Si una persona tiene que acercarse al robot (por ejemplo, caminar y pararse frente a él). **Penalty:** -150pts.
2. **Informes:** los puntos se anotan solo cuando el robot regresa al operador y entrega
el informe a tiempo.
3. **Descripción de una Persona:** La descripción de cada huésped debe ser única; es decir, no compartido
con cualquier otro invitado. Esto puede incluir ropa, edad, altura, complexión, color de piel, color de cabello, género, etc.
4. **Ubicación de una persona:** una ubicación relativa basada en el entorno (p. ej., el niño está
sentado en el carruaje, el hombre alto está de pie junto a la lámpara, etc.).

## Instrucciones del árbitro
Los árbitros necesitan:
- Asigne nombres a los invitados.
- Asegúrese de que cada persona tenga un nombre.
- Asegúrese de que las personas permanezcan en el lugar designado.

## Instrucciones del CO
2h antes del test:
- Anuncie la ubicación donde se instruirá al robot.
- Anunciar la sala en la que se lleva a cabo la prueba.

## Hoja de puntuacion
El tiempo máximo para esta prueba es de 5 minutos.

![image](https://user-images.githubusercontent.com/90789825/167893506-03bcd9d0-700f-471c-9462-edb518a2540c.png)

## Penalizaciones por no asistir
1. **Inscripción automática:** Todos los equipos se inscriben automáticamente para todas las pruebas.
2. **Anuncio:** Si un equipo no puede participar en una prueba (por cualquier motivo), el líder del equipo
tiene que anunciar esto al CO al menos 60 minutos antes de que comience el turno de prueba.
3. **Penalizaciones:**
   - Un equipo que no está presente en la posición de inicio cuando su prueba programada comienza. **Penalty:** el equipo ya no puede participar en la prueba.
   - Si el equipo no anunció que no iba a participar. **Penalty:** -250pts.

## Launch y Publisher del Monologo
El launch para lanzar solo el nodo monologo con el arbol de TextToSpeech.
```
roslaunch robocup_home_education_los_ultramarinos Main.launch
```

Los publicadores de cada estado.
```
rostopic pub /msg_receive std_msgs/String "Start"
rostopic pub /msg_receive std_msgs/String "Left"
rostopic pub /msg_receive std_msgs/String "Not_vision"
rostopic pub /msg_receive std_msgs/String "Stop"
```

Cambio en el /catkin_ws/src/dialog/dialogflow_ros/dialogflow_ros/scripts/dialogflow_ros/dialogflow_client.py
```py
# line 275: rospy.loginfo(output.print_result(response.query_result))
# line 324: rospy.loginfo(output.print_result(final_result))
```

## Publicar puntos en el navegation

```py
rostopic pub /move_base_simple/goal geometry_msgs/PoseStamped '{ header: {stamp: now, frame_id: "map"}, pose: { position: {x: 0, y: 0, z: 0.0}, orientation: {w: 1.0}}}'
```

## Roslint

```py
catkin_make roslint_robocup_home_education_los_ultramarinos
```
