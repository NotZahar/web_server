### Веб-сервер по раздаче статического контента

##### Features:
- Поддержка TLSv1.2
- Авторизованный доступ к контенту

##### Dependencies:
- Boost v1.82
- OpenSSL v3.0
- SQLiteCpp v3.3 [ref](https://github.com/SRombauts/SQLiteCpp)
- cpp-jwt v1.4 [ref](https://github.com/arun11299/cpp-jwt)
- (Bcrypt.cpp v2.0 [ref](https://github.com/hilch/Bcrypt.cpp) - compiled as third-party lib)
- CMake v3.28
- Doxygen v1.9

##### Build:
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

##### Docs:
- https://notzahar.github.io/web_server/
