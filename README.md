# Injector-Dll
Injector Dll sin usar nigun tipo de 3d party

# Como-usar
Simplemente es asegurarse que el proceso este corriendo y abierto, despues es ir dentro del codigo y copiar y pegar la ruta de tu archivo deseado a injectar, seguido de eso solamente queda precionar en Injectar, Teclear el nombre del proceso y seguido de ello seria injectar

# Detalles-a-fondo
Este injector esta hecho a base de Nt Create Thread Ex, utlizando la API nativa de windows.CreateThread esta me permite poder crear y ejectutar algun proceso como un .dll
porque decidi usar Nt Create Thread Ex? - Bien Esta funcion es perfecta ya que me permite poder injectar dentro del proceso sin que se tengan suficientes privilegios del sistema, que por cierto la escala de privilegios ya esta implementada en el codigo de todos modos
