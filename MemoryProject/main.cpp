#define _CRTDBG_MAP_ALLOC
#include <Standard_Handle.hxx>
#include <Standard_Transient.hxx>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <crtdbg.h>
#include <functional>
#include <vector>

class MyClass : public Standard_Transient
{
public:
    MyClass() { std::cout << "MyClass created" << std::endl; }
    ~MyClass() { std::cout << "MyClass destroyed" << std::endl; }
};

class Node
{
public:
    std::shared_ptr<Node> next; // 次のノードを共有
    std::weak_ptr<Node> prev;   // 前のノードを弱参照（循環参照を防ぐ）

    ~Node() { std::cout << "Node destroyed" << std::endl; }
};

class Child; // 前方宣言

class Parent : public Standard_Transient
{
public:
    Handle(Child) child; // 子を参照

    Parent()
    {
        std::cout << "Parent created" << std::endl;
    }

    ~Parent()
    {
        std::cout << "Parent destroyed" << std::endl;
    }
};

class Child : public Standard_Transient
{
public:
    // Parent *parent; // 親を生ポインタで参照（循環参照を防ぐ）
    Handle(Parent) parent; // 親を参照

    Child()
    {
        std::cout << "Child created" << std::endl;
    }

    ~Child()
    {
        std::cout << "Child destroyed" << std::endl;
    }
};

class SharedObject : public Standard_Transient
{
private:
    int value; // 管理する値

public:
    // コンストラクタ
    SharedObject(int val) : value(val) {}

    // 値を設定し、変更を通知
    void setValue(int val)
    {
        if (value != val)
        {
            value = val;
            notifyChange(); // 変更を通知
        }
    }

    // 値を取得
    int getValue() const
    {
        return value;
    }

private:
    // 変更を通知する
    void notifyChange()
    {
        std::cout << "SharedObject: Value changed to " << value << std::endl;
    }
};

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // int* leak = new int[10]; // 解放されないメモリ
    //   Handle(MyClass) handle1 = new MyClass(); // ヒープ上にオブジェクトを作成
    {
        // Handle(MyClass) handle2 = handle1; // 所有権を共有
        // std::cout << "Reference count: " << handle1->GetRefCount() << std::endl;

        Handle(Parent) parent = new Parent();
        Handle(Child) child = new Child();

        parent->child = child; // 親が子を参照
        // child->parent = parent.get(); // 子が親を生ポインタで参照
        child->parent = parent; // 子が親を参照（循環参照が発生する箇所）
        child->parent = parent; // 子が親を参照（循環参照が発生する箇所）
        std::cout << "Reference count of child: " << child->GetRefCount() << std::endl;
        std::cout << "Reference count of parent: " << parent->GetRefCount() << std::endl;

    } // handle2 がスコープを抜ける → 参照カウントが減少
    // std::cout << "Reference count: " << handle1->GetRefCount() << std::endl;

    // std::shared_ptr<Node> node1 = std::make_shared<Node>();
    // std::shared_ptr<Node> node2 = std::make_shared<Node>();

    // node1->next = node2; // node1 が node2 を共有
    // node2->prev = node1; // node2 が node1 を弱参照

    // ヒープメモリ上のインスタンスを共有
    Handle(SharedObject) object1 = new SharedObject(42);
    Handle(SharedObject) object2 = object1; // object1 と同じインスタンスを共有
    Handle(SharedObject) object3 = new SharedObject(50);

    std::cout << "Initial value (object1): " << object1->getValue() << std::endl;
    std::cout << "Initial value (object2): " << object2->getValue() << std::endl;
    std::cout << "Initial value (object3): " << object3->getValue() << std::endl;

    // object1 を通じて値を変更
    object1->setValue(100);
    object2->setValue(100);
    object2->setValue(200);

    std::cout << "End of main" << std::endl;
    _CrtDumpMemoryLeaks(); // メモリリークをチェック
    // OpenCASCADE のメモリリークチェック
    Standard::Purge(); // 未解放のオブジェクトを解放

    return 0; // handle1 がスコープを抜ける → オブジェクトが解放される
}
