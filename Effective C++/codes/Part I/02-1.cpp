#include <iostream>
class GamePlayer {
public:
    static const int NumTurns = 5;//this is just a declaration.
    GamePlayer(int val){
        for (int i = 0; i < NumTurns; ++i) scores[i] = val;
    }
private:
    int scores[NumTurns];
};
const int GamePlayer::NumTurns;
// this is the definition, only when this exists
// the address of the variable is existent
int main(){
    GamePlayer foo(10);
    std::cout << &foo.NumTurns << std::endl;
    return 0;
}