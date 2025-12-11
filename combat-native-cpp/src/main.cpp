#include "../include/main.hpp"

int main() {
    chooseInstance(BALANCED);

    Player p(1);   // crea un jugador aleatorio
    p.print();     // imprime sus stats

    return 0;
}
