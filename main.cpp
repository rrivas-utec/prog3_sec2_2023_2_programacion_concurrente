#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

void f1() {
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Mi primer hilo";
}

void ejemplo_1() {
    thread t1;
    t1 = thread(f1);
    cout << "Hola Mundo!\n";
    t1.join();
}

void ejemplo_2() {
    jthread t1(f1);
    cout << "Hola Mundo!\n";
}

void sumar(int a, int b, int& result) {
    result = a + b;
}

void restar(int a, int b, int* result) {
    *result = a - b;
}

// Hilos con funciones
void ejemplo_3() {
    int res = 0;
    jthread t1(sumar, 10, 20, ref(res));
    t1.join();
    cout << res << endl;
}

void ejemplo_4() {
    int res = 0;
    jthread t1(restar, 10, 20, &res);
    t1.join();
    cout << res << endl;
}
// Hilos con lambda
void ejemplo_3_lambda() {
    int res = 0;
    int x = 10;
    int y = 20;
    // Con parametro
    auto lambda = [&res](int a, int b){ sumar(a, b, res); };
    jthread t1(lambda, x, y);
    jthread t2([](int a, int b, int& res){ sumar(a, b, res); }, x, y, ref(res));
    // Sin parametros
    jthread t3([a=x, b=y, &res]{ sumar(a, b, res); });

    t1.join();
    cout << res << endl;
}

class A {
    jthread t1;
public:
    void metodo_sumar(int a, int b, int& result) {
        result = a + b;
    }
    void metodo_hilo() {
        int res {};
        t1 = jthread(&A::metodo_sumar, this, 10, 20, ref(res));
        t1.join();
        cout << res << endl;
    }
};

void ejemplo_5() {
    int res{};
    A obj;
    jthread t1(&A::metodo_sumar, &obj, 10, 20, ref(res));
    t1.join();
    cout << res << endl;
}

// Contenedores de hilos
void ejemplo_6() {
    vector<int> vr(3);
    vector<jthread> vt;
    vt.emplace_back(sumar, 10, 20, ref(vr[0]));
    vt.emplace_back(sumar, 100, 200, ref(vr[1]));
    vt.emplace_back(restar, 500, 200, &vr[2]);

    // Espera que terminen los hilos
    for (auto& t: vt) t.join();

    auto total = accumulate(begin(vr), end(vr), 0);
    cout << "total: " << total << endl;
}

int main() {
//    ejemplo_2();
//    ejemplo_3();
//    ejemplo_4();
    ejemplo_6();
    return 0;
}
