# Listas enlazadas en C++

Código del vídeo **[Listas enlazadas en C++: cómo funcionan de verdad](https://youtu.be/e_dwGjIa1Ro)**, de [Carlos Azaustre · AprendiendoDEV](https://www.youtube.com/@CarlosAzaustre).

Construimos una lista de tickets para entender qué ocurre al recorrer, insertar y borrar nodos. El objetivo es comprender los punteros, el orden de las asignaciones y quién se encarga de liberar la memoria, antes de pasar a los contenedores de la biblioteca estándar.

## Qué vas a aprender

- Representar un nodo con datos y un puntero al siguiente nodo.
- Recorrer una cadena hasta `nullptr` y buscar un ticket por su ID.
- Insertar sin perder el sucesor ni crear un ciclo.
- Reconectar los enlaces antes de liberar un nodo.
- Encapsular la propiedad de los nodos y la limpieza mediante RAII.
- Usar `std::forward_list` y `std::list`.
- Distinguir el coste de modificar un enlace del coste de llegar a la posición.

Necesitas conocer `struct`, funciones y bucles. Los ejemplos utilizan **C++17** y únicamente la biblioteca estándar.

## Contenido del repositorio

| Archivo | Qué muestra |
| --- | --- |
| [`ticket.hpp`](ticket.hpp) | `Ticket` (`id`, `title`) y `Node` (`ticket`, `next`). |
| [`01-nodes.cpp`](01-nodes.cpp) | Tres nodos con duración de almacenamiento automática y recorrido mediante punteros. |
| [`02-link-order.cpp`](02-link-order.cpp) | Un orden incorrecto que crea un ciclo, y el orden correcto para insertar. El ciclo se comprueba sin recorrerlo indefinidamente. |
| [`ticket-list.hpp`](ticket-list.hpp) | Contenedor propietario: inserción, búsqueda, borrado, limpieza iterativa y destructor. |
| [`03-ticket-list.cpp`](03-ticket-list.cpp) | Uso completo de `TicketList` con tickets 101, 102 y 103. |
| [`04-standard-containers.cpp`](04-standard-containers.cpp) | Inserción y borrado con `std::forward_list` y `std::list`. |
| [`05-count-search.cpp`](05-count-search.cpp) | Conteo de comparaciones al buscar el último nodo de listas de 4, 8 y 16 elementos. |
| [`tests.cpp`](tests.cpp) | Pruebas de lista vacía, inserción, borrado, búsqueda, estabilidad de nodos y destrucción de una lista de 100.000 elementos. |
| [`verify.sh`](verify.sh) | Compila y ejecuta los cinco ejemplos y las pruebas con warnings estrictos y sanitizers. |

Cada `.cpp` tiene su propio `main`: **compila los archivos por separado**.

## Empezar

Necesitas Git y un compilador compatible con C++17: Clang, GCC o MSVC. En macOS puedes instalar las herramientas de desarrollo con `xcode-select --install`; en Linux, utiliza el compilador de tu distribución. En Windows, utiliza las herramientas de C++ de Visual Studio o un entorno como WSL.

```bash
git clone https://github.com/carlosazaustre/listas-enlazadas-cpp.git
cd listas-enlazadas-cpp
mkdir -p build
c++ -std=c++17 -Wall -Wextra -Wpedantic 01-nodes.cpp -o build/01-nodes
./build/01-nodes
```

Salida del primer ejemplo:

```text
101 -> 102 -> 103 -> nullptr
```

Para ejecutar otro ejemplo, cambia el archivo y el nombre del binario:

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic 03-ticket-list.cpp -o build/03-ticket-list
./build/03-ticket-list
```

Con MSVC, desde su terminal de desarrollo:

```powershell
cl /std:c++17 /EHsc /W4 01-nodes.cpp /Fe:01-nodes.exe
.\01-nodes.exe
```

## Verificar todos los ejemplos

En macOS, Linux o WSL, con Clang instalado:

```bash
bash verify.sh
```

Para utilizar GCC:

```bash
CXX=g++ bash verify.sh
```

El script compila con `-Wall -Wextra -Wpedantic -Werror`, AddressSanitizer y UndefinedBehaviorSanitizer. Los binarios se generan en una carpeta temporal cuya ruta aparece al terminar. No requiere instalar librerías externas.

Las pruebas utilizan `assert`: no compiles con `-DNDEBUG` si quieres ejecutarlas. El soporte de sanitizers depende del compilador y del entorno. El código y las pruebas se han verificado con Clang en macOS.

## Las ideas importantes

### Conserva el sucesor antes de cambiar el enlace

```cpp
inserted.next = first.next;
first.next = &inserted;
```

Si primero haces `first.next = &inserted` y luego copias `first.next` a `inserted.next`, el nuevo nodo apunta a sí mismo. El ejemplo `02-link-order.cpp` reproduce este caso de forma controlada y después restaura la cadena.

### Reconecta antes de liberar

```cpp
Node* removed = previous.next;
previous.next = removed->next;
delete removed;
```

La lista mantiene el acceso al sucesor antes de destruir el nodo que se elimina. Después del `delete`, cualquier puntero a ese nodo deja de ser válido.

### RAII: la limpieza sigue la vida del objeto

**RAII** significa *Resource Acquisition Is Initialization*. En `TicketList`, el contenedor posee los nodos que reserva y su destructor llama a `clear()`. Cuando termina la vida del contenedor, libera los nodos restantes. La limpieza es iterativa para que la profundidad de la pila de llamadas no crezca con la longitud de la lista.

Los nodos de `01-nodes.cpp` y `02-link-order.cpp` tienen duración de almacenamiento automática; no se liberan con `delete`. En `TicketList`, los `new` y `delete` quedan encapsulados dentro del contenedor.

### Modificar un enlace y buscar la posición tienen costes distintos

| Operación en esta lista simple | Coste |
| --- | --- |
| Acceder a la cabeza / comprobar si está vacía | O(1) |
| Buscar por ID | O(n) |
| Recorrer toda la lista | O(n) |
| Insertar después de un nodo ya conocido | O(1) de trabajo estructural |
| Borrar después de un nodo ya conocido | O(1) de trabajo estructural |
| Insertar o eliminar al principio | O(1) de trabajo estructural |
| Vaciar / destruir la lista | O(n) |

Si necesitas buscar primero el nodo anterior por ID, esa operación completa puede ser O(n). Los costes estructurales no incluyen el coste de reservar memoria, construir o destruir el contenido del ticket. El ejemplo `05-count-search.cpp` cuenta comparaciones; **no es un benchmark de tiempo**.

## Contrato de `TicketList`

| Método | Comportamiento |
| --- | --- |
| `head()` | Devuelve un puntero observador constante a la cabeza. |
| `empty()` | Indica si la lista está vacía. |
| `pushFront(ticket)` | Reserva e inserta un nodo al principio. |
| `find(id)` | Devuelve el primer nodo con ese ID, o `nullptr`. |
| `insertAfter(previous, ticket)` | Inserta después de un nodo vivo que pertenece a esta lista. |
| `eraseAfter(previous)` | Elimina el sucesor; devuelve `false` si no existe. |
| `popFront()` | Elimina la cabeza; devuelve `false` si la lista está vacía. |
| `clear()` | Libera todos los nodos; puede llamarse sobre una lista vacía. |

Es una implementación didáctica. Los punteros que devuelve son observadores: no transfieren propiedad y no debes liberar sus nodos desde fuera. `insertAfter` y `eraseAfter` requieren que el nodo anterior pertenezca a esa misma lista. No se comprueba esa pertenencia mediante otra búsqueda.

Las inserciones mantienen válidas las direcciones de los nodos existentes. Al eliminar un nodo, los punteros que lo observan quedan invalidados; `clear()` y el destructor invalidan todos los observadores. Copia y movimiento están deshabilitados para evitar duplicar la propiedad de los nodos. No se proporcionan iteradores ni la interfaz completa de un contenedor estándar.

Para proyectos reales, estudia los contenedores estándar y elige según los patrones de acceso. Una lista enlazada no es automáticamente más rápida que `std::vector`: la localidad de memoria y la necesidad de buscar la posición también importan.

## Vídeo y resto del curso de C++

**[Ver el vídeo de este repositorio: listas enlazadas en C++](https://youtu.be/e_dwGjIa1Ro)**.

Continúa en la **[playlist del curso: fundamentos, estructuras de datos y algoritmos](https://www.youtube.com/playlist?list=PLUdlARNXMVkl7lnRLFm1fqfgcCc8ekOMI)**. Los enlaces siguientes siguen el recorrido del curso; los nombres resumen el tema de cada vídeo.

| Orden | Vídeo |
| --- | --- |
| 1 | [Por qué aprender C++ mejora tu forma de programar](https://youtu.be/3LdOOTnhFzg) |
| 2 | [Aprende C++ desde cero](https://youtu.be/R2zObBgWWa8) |
| 3 | [Cómo instalar C++ en tu ordenador](https://youtu.be/jIYucTgdzLQ) |
| 4 | [Condicionales y bucles: if, while y for](https://youtu.be/f-QhF-CkQpE) |
| 5 | [Paso por valor y por referencia](https://youtu.be/1Z36GrfXEns) |
| 6 | [Estructuras de datos: cuál usar y cuándo](https://youtu.be/LFOkPmHAOb8) |
| 7 | [Listas enlazadas en C++](https://youtu.be/e_dwGjIa1Ro) |

## Practica sobre el código

1. Busca un ID que no exista y comprueba el resultado.
2. Inserta al final utilizando un nodo anterior conocido.
3. Elimina la cabeza, un nodo intermedio y el último nodo.
4. Amplía las pruebas antes de incorporar una nueva operación.
5. Compara el número de comparaciones al buscar el primer y el último ticket.

## Autor y recursos

Material de **Carlos Azaustre**, para aprender a desarrollar software entendiendo cómo funciona por dentro.

- [YouTube · AprendiendoDEV](https://www.youtube.com/@CarlosAzaustre)
- [Cursos de programación](https://aprendiendo.dev)
- [Web y newsletter](https://carlosazaustre.es)
- [Libros](https://carlosazaustre.es/libros)
