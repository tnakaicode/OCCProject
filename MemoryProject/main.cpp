#include <Standard_Handle.hxx>
#include <Standard_Transient.hxx>
#include <iostream>
#include <memory>

class MyClass : public Standard_Transient {
public:
    MyClass() { std::cout << "MyClass created" << std::endl; }
    ~MyClass() { std::cout << "MyClass destroyed" << std::endl; }
};

class Node {
    public:
        std::shared_ptr<Node> next; // 次のノードを共有
        std::weak_ptr<Node> prev;   // 前のノードを弱参照（循環参照を防ぐ）
    
        ~Node() { std::cout << "Node destroyed" << std::endl; }
    };

int main() {
    Handle(MyClass) handle1 = new MyClass(); // ヒープ上にオブジェクトを作成
    {
        Handle(MyClass) handle2 = handle1; // 所有権を共有
        std::cout << "Reference count: " << handle1->GetRefCount() << std::endl;
    } // handle2 がスコープを抜ける → 参照カウントが減少
    std::cout << "Reference count: " << handle1->GetRefCount() << std::endl;

    std::shared_ptr<Node> node1 = std::make_shared<Node>();
    std::shared_ptr<Node> node2 = std::make_shared<Node>();

    node1->next = node2; // node1 が node2 を共有
    node2->prev = node1; // node2 が node1 を弱参照

    return 0; // handle1 がスコープを抜ける → オブジェクトが解放される
}
