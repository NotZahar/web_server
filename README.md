### Веб-сервер по раздаче статического контента

##### Features:
- Поддержка TLSv1.2
- Авторизованный доступ к контенту

##### Dependencies:
- Boost v1.82
- OpenSSL v3.0
- SQLiteCpp v3.3
- cpp-jwt v1.4
- CMake v3.28
- Doxygen v1.9

##### Build:
- mkdir build && cd build
- cmake -DCMAKE_BUILD_TYPE=Release ..
- cmake --build .

##### Docs:
https://notzahar.github.io/web_server/
