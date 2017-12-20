# wilton_test_server
Тестовый сервер на основе библиотеки wilton_server. С использованием wilton_server C API.

### Положение директории
Предполагается следующее расположение:
```
  some_catalog
    wilton
      ...
    wilton_test_server
      ...
  
```
### Сборка с ключами -g -Og

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

## Проверка производительности различными утилитами

### Запуск callgrind
```
valgrind --tool=callgrind ./test_server
```
### Запуск perf
```
sudo perf record -F 99 -g -- ./test_server 
```
### Запуск нагрузки под wrk
```
./wrk -t1 -c400 -d30s -s./scripts/post.lua http://127.0.0.1:8080/index.html
```
### Генерация FlameGraph
```
sudo perf script | ../../../utils/FlameGraph/stackcollapse-perf.pl | ../../../utils/FlameGraph/flamegraph.pl > perf.svg
```
