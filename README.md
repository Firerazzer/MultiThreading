# MultiThreading
Projet de MultiThreading et Safety des systèmes informatiques

# Download project
'''git clone https://github.com/FlorentLabrune/MultiThreading.git'''

# Build project
cd MultiThreading
'''./build.sh'''

# Run project
lancer le capteur
'''./capteur/bin/Capteur'''

lancer le watchdog
'''./watchdog/bin/Watchdog'''

lancer le service en mode primary
'''./service/bin/Service primary'''
OU en mode backup
'''./service/bin/Service backup'''
OU en mode injection d'erreur
'''./service/bin/Service [primary|backup] f'''

# Notes
Il est judicieux de lancer ces commandes dans des terminaux différents pour voir clairement les interactions
Le main du service est un main de TEST. Il est donc prévu pour qu'un service en mode primary s'arrete au bout de 7s de fonctionnement. Il est libre d'implémenter l'objet Service.cpp de maniere adapte a votre application.
Si un service en mode backup passe en mode primary il n'est pas possible d'en relancer un en mode backup
Il est possible de lancer un service en mode primary sans watchdog MAIS si le watchdog est tué pendant que des services tournent alors ils planteront tous