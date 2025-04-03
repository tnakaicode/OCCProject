---
title: OpenCASCADE
---

## **C++ における `.` と `->` の違い**

C++ では、オブジェクトやポインタを通じてメンバーやメソッドにアクセスする際に、`.`（ドット演算子）と `->`（アロー演算子）を使い分けます。

### **基本的な違い**

- **`.`（ドット演算子）**
  - 通常のオブジェクト（値型）を通じてメンバーやメソッドにアクセスします。
  - 例：

```cpp
MyClass obj;
obj.method(); // オブジェクトを通じてメソッドを呼び出す
```

- **`->`（アロー演算子）**
  - ポインタを通じてメンバーやメソッドにアクセスします。
  - 例：

```cpp
MyClass* objPtr = new MyClass();
objPtr->method(); // ポインタを通じてメソッドを呼び出す
```

---

## **OpenCASCADE における `.` と `->` の使い分け**

OpenCASCADE では、多くのクラスが `Handle`（スマートポインタ）を使用して管理されています。`Handle` はポインタのように動作するため、`->` を使用してメソッドやメンバーにアクセスします。

### **例: `label.FindAttribute`**

`label` は通常のオブジェクト（`TDF_Label` 型）であるため、`.` を使用してメソッドにアクセスします。

```cpp
TDF_Label label;
Handle(TDF_Attribute) attribute;
if (label.FindAttribute(guid, attribute)) {
    // 属性を取得
}
```

### **例: `shapeTool->GetShape`**

`shapeTool` は `Handle(XCAFDoc_ShapeTool)` 型（スマートポインタ）であるため、`->` を使用してメソッドにアクセスします。

```cpp
Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
TopoDS_Shape shape = shapeTool->GetShape(label);
```

---

## **`.*` と `->*` の特殊なケース**

`.*` と `->*` は、メンバーポインタを使用する場合にのみ使用されます。これは、通常のメソッド呼び出しとは異なる特殊なケースです。

### **例: メンバーポインタの使用**

```cpp
#include <iostream>

class MyClass {
public:
    void method() {
        std::cout << "Method called!" << std::endl;
    }
};

int main() {
    MyClass obj;
    void (MyClass::*methodPtr)() = &MyClass::method; // メンバーポインタを取得

    // オブジェクトを通じてメンバーポインタを呼び出す
    (obj.*methodPtr)();

    MyClass* objPtr = &obj;
    // ポインタを通じてメンバーポインタを呼び出す
    (objPtr->*methodPtr)();

    return 0;
}
```

**出力:**

```bash
Method called!
Method called!
```

---

## **まとめ**

| **ケース**                      | **アクセス方法** | **例**                                  |
| ------------------------------- | ---------------- | --------------------------------------- |
| 通常のオブジェクト（値型）      | `.`              | `label.FindAttribute(guid, attribute);` |
| ポインタ（生ポインタ）          | `->`             | `objPtr->method();`                     |
| スマートポインタ（`Handle` 型） | `->`             | `shapeTool->GetShape(label);`           |

OpenCASCADE では、`Handle` 型が多く使用されるため、`->` を使うケースが一般的です。一方、`TDF_Label` のような通常のオブジェクトでは `.` を使用します。どちらを使うべきかは、変数がオブジェクトかポインタかによって決まります。

---

## **デバッグ情報**

以下は、アプリケーション実行時のデバッグログの一部です。

```log
'QtProject.exe' (Win32): Loaded 'C:\*******\x64\Debug\QtProject.exe'. Symbols loaded.
'QtProject.exe' (Win32): Loaded 'C:\Windows\System32\ntdll.dll'. 
'QtProject.exe' (Win32): Loaded 'C:\Windows\System32\kernel32.dll'. 
...
Unhandled exception at 0x00007FFDE16D286E (ucrtbase.dll) in QtProject.exe: Fatal program exit requested.
```

このログは、アプリケーションがロードしたモジュールやエラーの詳細を示しています。エラーが発生した場合、ここに記録された情報をもとに問題を特定してください。
