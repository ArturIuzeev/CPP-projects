# perf

В этой задаче вам дан набор бенчмарков.
В каждом бенчмарке есть performance bug.
Вы должны найти этот баг, используя профилировщик, и исправить его.

**В описании PR укажите, как именно нашли проблему в каждом случае.
Приложите скриншоты релевантных результатов профилировщика.**

Менять суть алгоритмов запрещается, только их реализацию.
Также нельзя менять функции, отвечающие за сами бенчмарки (начинаются с `bm_`).

Задания:
- `jpeg` - найти bottleneck и ускорить бенчмарк в 6 раз.
- `pi` - найти bottleneck и ускорить бенчмарк в 50 раз.
- `radix_sort` - найти bottleneck и ускорить бенчмарк на 30%.
- `json` - понять, что тормозит, и ускорить бенчмарк в 2 раза, не меняя сами структуры.
- `logger` - найти bottleneck и ускорить бенчмарк в 40 раз. Смысл класса `Logger` должен остаться прежним. Содержимое лога должно остаться таким же, как и до ваших изменений.

**Важно:** профилировать нужно с пресетом `RelWithDebInfo`.
Не забудьте выбрать его, если используете CLion.
Руками его можно выставить, передав в CMake аргумент `--preset RelWithDebInfo`.
Также результат может существенно отличаться на разных компиляторах, в CI используется GCC.

## perf 

Если вы используете `perf`, возможно, вам помогут команды `perf record` и `perf report`, а также флаги `-g` и `--call-graph dwarf`.
Ещё полезное про `perf`: https://www.brendangregg.com/perf.html.

Можно использовать и другие профилировщики, однако далее будет рассмотрен лишь сценарий использования `perf`.

### FlameGraph

Для анализа результатов `perf` можно пользоваться как обычным консольным `perf report`, так и более удобным [FlameGraph](https://github.com/brendangregg/FlameGraph).

Чтобы нарисовать флейм-граф, нужно:
1. Склонить его куда-нибудь (`git clone https://github.com/brendangregg/FlameGraph.git`)
2. В директории с собранным `perf.data` прогнать команду `perf script > out.perf` (важно, чтобы профиль был собран с флагом `-g`)
3. Находясь в директории, куда склонили FlameGraph, запустить команду `FlameGraph/stackcollapse-perf.pl <path to out.perf> > out.folded && FlameGraph/flamegraph.pl out.folded > fgraph.svg`

### gprof2dot

Ещё один способ визуализации - [gprof2dot](https://github.com/jrfonseca/gprof2dot), превращающий профиль в dot-граф. 
Запускает командой по типу `gprof2dot -f perf -o out.dot out.perf`, где `out.perf` - результат запуска `perf script` (см. выше часть про FlameGraph).
 
## Сборка и запуск perf

> [!WARNING]
> **Работоспособность гарантируется только на Linux.**
> На системах, отличных от Linux, придётся использовать либо виртуалку (например, WSL), либо Docker.

### Опции защиты ядра

По умолчанию в вашей системе могут быть установлены опции ядра, ограничивающие работу `perf`, в частности `perf_event_paranoid` и `kptr_restrict`.
Их можно изменить до следующего перезапуска системы:
```shell
sudo sysctl kernel.perf_event_paranoid=1
sudo sysctl kernel.kptr_restrict=0
```

Более подробную информацию о них можно найти [здесь](https://www.jetbrains.com/help/clion/cpu-profiler.html#Prerequisites). 

### Использование Docker

Инструкцию по установке и настройке докера можно найти в [CPP-KT/containers](https://github.com/CPP-KT/containers).

Чтобы `perf` работал внутри докера, нужно добавить опцию `--privileged` к `docker run`.

Если вы пользуетесь Windows, то, чтобы заработал `perf` в докер-контейнере, нужно сделать следующие шаги:
1. Убедиться, что в Docker Desktop установлен WSL2 как бекенд https://docs.docker.com/desktop/windows/wsl/
2. Cделать `wsl -l -v`, увидеть там работающий инстанс docker-desktop
3. Cделать `wsl -d docker-desktop`
4. Вы попадёте внутрь шелла WSL, там нужно выполнить `sysctl kernel.perf_event_paranoid=-1`
5. perf внутри докер-контейнера должен заработать.

Если вы пользуетесь MacOS, то возможно единственный путь это запускать докер-контейнер без указания user'а (то есть под рутом).

### Сборка руками

В этой задаче, как обычно, работает сборка и запуск [через CLion](https://cpp-kt.github.io/course/ide/clion.html), но возможностей встроенного в CLion профайлера вам может не хватить.

Чтобы собрать руками, можно выполнить следующие команды (при условии, что у вас уже настроено окружение аналогичное тому, что используется для запуска через CLion, то есть установлен vcpkg).

```sh
mkdir -p cmake-build-RelWithDebInfo
rm -rf cmake-build-RelWithDebInfo/*
cmake "-DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/vcpkg/scripts/buildsystems/vcpkg.cmake" -GNinja --preset RelWithDebInfo -S .
cmake --build cmake-build-RelWithDebInfo
```

Бинари для каждой из подзадач будут лежать в `cmake-build-RelWithDebInfo/perf`, дальше их можно использовать для запуска под `perf`.
