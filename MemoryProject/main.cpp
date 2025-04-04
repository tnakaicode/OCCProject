#define _CRTDBG_MAP_ALLOC
#include <Standard_Handle.hxx>
#include <Standard_Transient.hxx>
#include <iostream>
#include <memory>
#include <cstdlib>
#include <crtdbg.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>

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

// TrackedHandle クラスの定義
template <typename T>
class TrackedHandle : public Handle(T)
{
private:
    static thread_local TrackedHandle *currentHandle; // スレッドローカルで現在の Handle を追跡
    int handleId;                                     // 各 Handle に一意の ID を割り当て
    std::string handleName;                           // 各 Handle に名前を付ける
    static int nextHandleId;                          // 次に割り当てる Handle ID

public:
    // コンストラクタ
    TrackedHandle(T * ptr, const std::string &name)
        : Handle(T)(ptr), handleId(nextHandleId++), handleName(name) {}

    // コピーコンストラクタ
    TrackedHandle(const TrackedHandle &other, const std::string &name)
        : Handle(T)(other), handleId(nextHandleId++), handleName(name) {}

    // 操作中の Handle を設定
    T *operator->()
    {
        currentHandle = this;
        return Handle(T)::operator->();
    }

    // 現在の Handle を取得
    static TrackedHandle *getCurrentHandle()
    {
        return currentHandle;
    }

    // Handle ID を取得
    int getHandleId() const
    {
        return handleId;
    }

    // Handle 名を取得
    const std::string &getHandleName() const
    {
        return handleName;
    }
};

// スレッドローカル変数の初期化
template <typename T>
thread_local TrackedHandle<T> *TrackedHandle<T>::currentHandle = nullptr;

// 次に割り当てる Handle ID の初期化
template <typename T>
int TrackedHandle<T>::nextHandleId = 1;

// SharedObject クラスの定義
class SharedObject : public Standard_Transient
{
private:
    int value;                 // 管理する値
    int instanceId;            // 各インスタンスに一意の ID を割り当てる
    static int nextInstanceId; // 次に割り当てるインスタンス ID

public:
    // コンストラクタ
    SharedObject(int val) : value(val), instanceId(nextInstanceId++) {}

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

    // インスタンス ID を取得
    int getInstanceId() const
    {
        return instanceId;
    }

    // 変更を通知する
    void notifyChange()
    {
        TrackedHandle<SharedObject> *currentHandle = TrackedHandle<SharedObject>::getCurrentHandle();
        if (currentHandle)
        {
            const std::string &handleName = currentHandle->getHandleName();
            std::cout << "SharedObject: Value changed to " << value
                      << " by Handle " << handleName
                      << " (Instance ID: " << instanceId << ")" << std::endl;
        }
        else
        {
            std::cout << "SharedObject: Value changed to " << value
                      << " by an unknown Handle (Instance ID: " << instanceId << ")" << std::endl;
        }
    }
};

// 静的メンバ変数の定義
int SharedObject::nextInstanceId = 1;

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
    TrackedHandle<SharedObject> object1 = TrackedHandle<SharedObject>(new SharedObject(42), "Object1");
    TrackedHandle<SharedObject> object2 = TrackedHandle<SharedObject>(object1, "Object2"); // object1 と同じインスタンスを共有
    TrackedHandle<SharedObject> object3 = TrackedHandle<SharedObject>(new SharedObject(50), "Object3");

    std::cout << "Initial value (object1): " << object1->getValue() << std::endl;
    std::cout << "Initial value (object2): " << object2->getValue() << std::endl;
    std::cout << "Initial value (object3): " << object3->getValue() << std::endl;

    // object1 を通じて値を変更
    object1->setValue(100); // SharedObject: Value changed to 100 by Handle Object1 (Instance ID: 1)
    object2->setValue(100); // 変更なし
    object2->setValue(200); // SharedObject: Value changed to 200 by Handle Object2 (Instance ID: 1)
    object3->setValue(200); // SharedObject: Value changed to 200 by Handle Object3 (Instance ID: 2)

    std::cout << "End of main" << std::endl;
    _CrtDumpMemoryLeaks(); // メモリリークをチェック
    // OpenCASCADE のメモリリークチェック
    Standard::Purge(); // 未解放のオブジェクトを解放

    return 0; // handle1 がスコープを抜ける → オブジェクトが解放される
}
