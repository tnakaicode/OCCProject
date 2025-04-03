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

## **オブジェクトとHandleの違い**

C++ におけるオブジェクトと OpenCASCADE の `Handle`（スマートポインタ）の違いについて説明します。

| **ケース**                      | **アクセス方法** | **例**                                  |
| ------------------------------- | ---------------- | --------------------------------------- |
| 通常のオブジェクト（値型）      | `.`              | `label.FindAttribute(guid, attribute);` |
| ポインタ（生ポインタ）          | `->`             | `objPtr->method();`                     |
| スマートポインタ（`Handle` 型） | `->`             | `shapeTool->GetShape(label);`           |

---

### **オブジェクト**

オブジェクトは、クラスのインスタンスであり、通常の値型としてスタックやヒープに直接格納されます。

#### **特徴**

- **直接的なメモリ管理**:
  - スタック上のオブジェクトはスコープを抜けると自動的に破棄されます。
  - ヒープ上に動的に割り当てた場合は、`delete` を使用して手動で解放する必要があります。
- **値型として扱う**:
  - オブジェクトは直接操作され、ポインタや参照を使わずにアクセスできます。

#### **例**

```cpp
TDF_Label label; // 通常のオブジェクト
Handle(TDF_Attribute) attribute;
if (label.FindAttribute(guid, attribute)) {
    // 属性を取得
}
```

### **Handle（スマートポインタ）**

OpenCASCADE の Handle は、参照カウントを使用したスマートポインタの一種です。Handle は、動的に割り当てられたオブジェクトを安全に管理し、メモリリークを防ぎます。

#### **特徴**

- **参照カウントによるメモリ管理**:
  - Handle は、内部で参照カウントを管理します。
  - 参照カウントが 0 になると、自動的にメモリが解放されます。
- **ポインタのように動作**:
  - Handle はポインタのように動作し、-> 演算子を使用してメソッドやメンバーにアクセスします。
- **共有所有権**:
  - 複数の Handle が同じオブジェクトを共有できます。

#### **例**

```cpp
Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
TopoDS_Shape shape = shapeTool->GetShape(label); // Handle を通じて形状を取得
```

### **オブジェクトと Handle の使い分け**

- オブジェクトを使うべき場合
  - 軽量で短命なデータや、スタック上で管理できるデータを扱う場合。
  - 例: TDF_Label、gp_Pnt、gp_Vec など。
- Handle を使うべき場合
  - 長命で共有されるリソースや、動的に割り当てられるオブジェクトを扱う場合
  - 例: XCAFDoc_ShapeTool、TopoDS_Shape、AIS_Shape など。

### **メモリ管理の違い**

| **項目**             | **オブジェクト**                               | **Handle（スマートポインタ）**             |
| -------------------- | ---------------------------------------------- | ------------------------------------------ |
| メモリ管理           | スタック上で自動管理、またはヒープ上で手動管理 | 参照カウントによる自動管理                 |
| 所有権               | 単一の所有権                                   | 複数の Handle で共有可能                   |
| メモリリークのリスク | 手動管理の場合、メモリリークのリスクがある     | 自動管理のため、メモリリークのリスクが低い |

### **OpenCASCADE における具体例**

TDF_Label（オブジェクト）
TDF_Label は軽量なクラスであり、通常のオブジェクトとして扱います。

```cpp
TDF_Label label;
Handle(TDF_Attribute) attribute;
if (label.FindAttribute(guid, attribute)) {
    // 属性を取得
}
```

XCAFDoc_ShapeTool（Handle）
XCAFDoc_ShapeTool は、Handle を使用して管理されるクラスです。

```cpp
Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
TopoDS_Shape shape = shapeTool->GetShape(label);
```

## OpenCASCADEのメモリ管理

OpenCASCADE では、メモリ管理が自動化されており、通常は開発者が手動でメモリを管理する必要はありません。`Handle`（スマートポインタ）やスタック上のオブジェクトを使用することで、メモリリークや解放忘れを防ぐ仕組みが整っています。

---

### **1. メモリ管理の基本**

#### **Handle（スマートポインタ）**

- OpenCASCADE の多くのクラス（`Standard_Transient` を継承するクラス）は、`Handle` を使用して管理されます。
- `Handle` は参照カウントを使用しており、参照カウントが 0 になると自動的にメモリが解放されます。
- 例：

    ```cpp
    Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
    // メモリは自動的に管理される
    ```

#### **スタック上のオブジェクト**

 軽量なクラス（例: `gp_Pnt`, `gp_Vec`, `TDF_Label` など）は、スタック上で管理されます。

- スタック上のオブジェクトはスコープを抜けると自動的に破棄されるため、手動で解放する必要はありません。
- 例：

    ```cpp
    gp_Pnt point(0.0, 0.0, 0.0); // スタック上に作成
    ```

---

### **2. 手動でメモリ管理が必要な場合**

以下のような状況では、開発者がメモリ管理を意識する必要があります。

#### **a. `new` を使用して動的に割り当てたオブジェクト**

- `Handle` を使用せずに `new` を直接使用して動的に割り当てた場合、手動で解放する必要があります。
- 例：

    ```cpp
    XCAFDoc_ShapeTool* shapeTool = new XCAFDoc_ShapeTool();
    // 手動で解放が必要
    delete shapeTool;
    ```

#### **b. `Standard_Transient` を継承しないクラス**

- OpenCASCADE の一部のクラス（軽量なクラスやユーティリティクラスなど）は、`Standard_Transient` を継承していないため、`Handle` を使用できません。
- これらのクラスを動的に割り当てた場合、手動で解放する必要があります。
- 例：

    ```cpp
    gp_Pnt* point = new gp_Pnt(0.0, 0.0, 0.0);
    // 手動で解放が必要
    delete point;
    ```

#### **c. 外部ライブラリとの連携**

- OpenCASCADE を外部ライブラリや独自のメモリ管理システムと統合する場合、手動でメモリを管理する必要がある場合があります。

#### **d. 特定の低レベル API の使用**

- OpenCASCADE の一部の低レベル API（例: `malloc` や `Standard::Allocate` を使用する場合）では、手動でメモリを解放する必要があります。
- 例：

    ```cpp
    void* rawMemory = Standard::Allocate(1024); // 1KB のメモリを確保
    // 手動で解放が必要
    Standard::Free(rawMemory);
    ```

---

### **3. メモリ管理のベストプラクティス**

1. **`Handle` を使用する**
   - OpenCASCADE のクラスが `Handle` に対応している場合は、必ず `Handle` を使用してメモリ管理を自動化します。
   - 例：

     ```cpp
     Handle(AIS_Shape) aisShape = new AIS_Shape(shape);
     m_context->Display(aisShape, Standard_True);
     ```

2. **スタック上のオブジェクトを優先する**
   - 軽量なクラス（例: `gp_Pnt`, `gp_Vec`）は、スタック上で管理することで安全かつ効率的に使用できます。
   - 例：

     ```cpp
     gp_Pnt point(1.0, 2.0, 3.0);
     ```

3. **`new` を避ける**
   - 可能な限り `new` を直接使用せず、`Handle` やスタック上のオブジェクトを使用します。

4. **低レベル API の使用を最小限にする**
   - `Standard::Allocate` や `malloc` などの低レベル API を使用する場合は、必ず対応する解放処理を実装します。

---

### **4. まとめ**

- **通常は自動管理**
  - OpenCASCADE の多くのクラスは `Handle` を使用して参照カウントを管理するため、手動で解放する必要はありません。
  - 軽量なクラスはスタック上で管理されるため、スコープを抜けると自動的に破棄されます。

- **手動でメモリ管理が必要なケース**
  - `new` を直接使用した場合。
  - `Standard_Transient` を継承していないクラスを動的に割り当てた場合。
  - 外部ライブラリや低レベル API を使用する場合。

OpenCASCADE を使用する際は、`Handle` を積極的に活用し、手動でのメモリ管理を最小限に抑えることが推奨されます。

## スタックメモリとヒープメモリの違い

C++ では、メモリの割り当て方法として **スタックメモリ** と **ヒープメモリ** の2種類があります。それぞれの特徴や用途について説明します。

---

### **1. スタックメモリ**

#### **特徴**

- **メモリの割り当て場所**: スタック（Stack）に割り当てられます。
- **メモリの管理**: 自動的に管理されます。
  - スタック上の変数はスコープを抜けると自動的に破棄され、メモリが解放されます。
- **速度**: スタックメモリの割り当てと解放は非常に高速です。
- **制約**: スタックのサイズは限られているため、大きなデータを扱う場合には不向きです。

#### **用途**

- 短命なデータや軽量なデータに適しています。
  - 例: 関数内のローカル変数や軽量なクラス（`gp_Pnt`, `gp_Vec` など）。

    ```cpp
    #include <iostream>

    void createObject() {
        int x = 10; // スタック上にメモリを割り当て
        std::cout << "Value of x: " << x << std::endl;
    } // スコープを抜けると x は自動的に破棄される
    ```

---

### **2. ヒープメモリ**

#### **特徴**

- **メモリの割り当て場所**: ヒープ（Heap）に割り当てられます。
- **メモリの管理**: 手動で管理する必要があります。
  - `new` を使って割り当てたメモリは、`delete` を使って明示的に解放する必要があります。
  - 解放を忘れるとメモリリークが発生します。
- **速度**: ヒープメモリの割り当てと解放はスタックよりも遅いです。
- **柔軟性**: ヒープはスタックよりも大きなメモリ領域を持つため、大きなデータや動的なデータ構造に適しています。

#### **用途**

- 長命なデータや動的にサイズが変わるデータに適しています。
  - 例: 動的配列や OpenCASCADE のオブジェクト（`Handle` を使用する場合）。

    ```cpp
    #include <iostream>

    void createObject() {
        int* x = new int(10); // ヒープ上にメモリを割り当て
        std::cout << "Value of x: " << *x << std::endl;
        delete x; // メモリを手動で解放
    }
    ```

---

### **3. スタックメモリとヒープメモリの比較**

| **項目**               | **スタックメモリ**                              | **ヒープメモリ**                              |
|------------------------|-----------------------------------------------|---------------------------------------------|
| **メモリの割り当て場所** | スタック                                      | ヒープ                                       |
| **メモリの管理**         | 自動（スコープを抜けると自動解放）              | 手動（`delete` を使って明示的に解放が必要）   |
| **速度**               | 高速                                         | 遅い                                        |
| **メモリサイズの制限**   | スタックサイズに依存（通常数 MB 程度）           | 制限が少ない（システムのメモリ容量に依存）    |
| **用途**               | 短命なデータや軽量なデータ                     | 長命なデータや動的にサイズが変わるデータ     |
| **メモリリークのリスク** | なし                                         | `delete` を忘れるとメモリリークが発生する     |

---

### **4. OpenCASCADE におけるメモリ管理**

OpenCASCADE では、`Handle`（スマートポインタ）を使用することで、ヒープメモリの管理を自動化しています。

- `Handle` は参照カウントを使用しており、参照カウントが 0 になると自動的にメモリが解放されます。
- 開発者が手動で `delete` を呼び出す必要はありません。

```cpp
#include <AIS_Shape.hxx>
#include <TopoDS_Shape.hxx>

void createShape(const TopoDS_Shape& shape) {
    Handle(AIS_Shape) aisShape = new AIS_Shape(shape); // Handle を使用
    // メモリ管理は自動化される
}
```

ヒープメモリとスタックメモリの最大量を確認する方法は、使用しているオペレーティングシステムや開発環境によって異なります。以下に、一般的な方法を説明します。

---

## **1. スタックメモリの最大量を確認する方法**

### **Windows**

- スタックサイズは、Visual Studio などのビルド設定で指定できます。
- デフォルトでは、スタックサイズは **1MB** に設定されていますが、変更可能です。

#### **確認方法**

1. **Visual Studio のプロジェクト設定で確認**
   - プロジェクトのプロパティを開きます。
   - **[リンカー] → [システム] → [スタックの予約サイズ]** を確認します。

2. **コマンドラインで確認**
   - ビルドされた実行ファイルのスタックサイズを確認するには、`dumpbin` コマンドを使用します。
   - 例：

     ```cmd
     dumpbin /headers your_program.exe
     ```

     出力の中に `stack reserve size` という項目があり、スタックサイズが表示されます。

3. **プログラム内で確認**
   - スタックサイズをプログラム内で確認することは直接的にはできませんが、再帰関数を使ってスタックの限界を調べることができます。
   - 例：

     ```cpp
     #include <iostream>

     void testStackSize(int depth) {
         char buffer[1024]; // スタック上に割り当て
         std::cout << "Depth: " << depth << std::endl;
         testStackSize(depth + 1); // 再帰呼び出し
     }

     int main() {
         try {
             testStackSize(1);
         } catch (...) {
             std::cout << "Stack overflow occurred!" << std::endl;
         }
         return 0;
     }
     ```

     ※ 実行するとスタックオーバーフローが発生し、スタックの限界を推測できます。

---

### **Linux**

- スタックサイズは、`ulimit` コマンドで確認できます。

#### **確認方法**

1. **ターミナルで確認**
   - 以下のコマンドを実行します：

     ```bash
     ulimit -s
     ```

     出力結果がスタックサイズ（KB単位）です。例えば、`8192` と表示された場合、スタックサイズは **8MB** です。

2. **スタックサイズを変更**
   - スタックサイズを変更するには、以下のコマンドを使用します：

     ```bash
     ulimit -s [サイズ]
     ```

     例：スタックサイズを 16MB に設定する場合：

     ```bash
     ulimit -s 16384
     ```

---

## **2. ヒープメモリの最大量を確認する方法**

### **Windows**

- ヒープメモリの最大量は、システムの物理メモリと仮想メモリの設定に依存します。
- プログラム内でヒープメモリの限界を確認するには、動的にメモリを割り当て続けて、どこで失敗するかを調べる方法があります。

#### **プログラムで確認**

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<void*> allocations;
    size_t allocated = 0;

    try {
        while (true) {
            void* block = malloc(1024 * 1024); // 1MB を割り当て
            if (!block) break;
            allocations.push_back(block);
            allocated++;
            std::cout << "Allocated: " << allocated << " MB" << std::endl;
        }
    } catch (...) {
        std::cout << "Heap allocation failed!" << std::endl;
    }

    for (void* block : allocations) {
        free(block); // メモリを解放
    }

    std::cout << "Total allocated: " << allocated << " MB" << std::endl;
    return 0;
}
```

- このプログラムは、1MB 単位でヒープメモリを割り当て続け、どこで失敗するかを確認します。

---

### **Linux**

- ヒープメモリの最大量は、システムの物理メモリとスワップ領域に依存します。
- `ulimit` コマンドでプロセスごとのメモリ制限を確認できます。

#### **確認方法**

1. **ターミナルで確認**
   - 以下のコマンドを実行します：

     ```bash
     ulimit -v
     ```

     出力結果が仮想メモリの最大量（KB単位）です。

2. **プログラムで確認**
   - Windows の例と同様に、動的にメモリを割り当て続けて、どこで失敗するかを調べることができます。

---

## **3. スタックとヒープの違いを考慮した設計**

- **スタックメモリ**
  - スタックサイズは比較的小さい（通常 1MB ～ 8MB 程度）。
  - 再帰呼び出しや大きなローカル変数を使用する場合は注意が必要です。

- **ヒープメモリ**
  - ヒープサイズはシステムの物理メモリと仮想メモリに依存するため、スタックよりも大きなメモリを扱えます。
  - 動的に割り当てる場合は、メモリリークを防ぐために適切に解放する必要があります。

---

## スタックメモリの設定方法

これで、スタックメモリとヒープメモリの最大量を確認する方法がわかります。システムやプログラムの要件に応じて、適切なメモリ管理を行いましょう！

Visual Studio のプロジェクトでスタックメモリサイズを 18MB に設定するには、プロジェクトのプロパティでスタックサイズを変更する必要があります。以下に手順を示します。

---

### **手順: Visual Studio プロジェクトでスタックメモリを 18MB に設定**

1. **プロジェクトのプロパティを開く**
   - Visual Studio でプロジェクトを開きます。
   - メニューバーから **[プロジェクト] → [プロパティ]** を選択します。

2. **リンカー設定を開く**
   - プロパティ ページで、左側のツリーから以下を選択します：
     **[構成プロパティ] → [リンカー] → [システム]**

3. **スタックの予約サイズを設定**
   - **[スタックの予約サイズ]** の項目を見つけます。
   - ここに **18874368**（18MB をバイト単位で指定）を入力します。
     - 1MB = 1024 × 1024 バイトなので、18MB = 18 × 1024 × 1024 = **18874368 バイト**。

4. **スタックのコミットサイズ（任意）**
   - 必要に応じて **[スタックのコミットサイズ]** も設定します。
     - 通常はデフォルト値のままで問題ありません。

5. **設定を保存**
   - **[OK]** をクリックしてプロパティを保存します。

6. **プロジェクトをビルド**
   - プロジェクトを再ビルドして、設定が反映されていることを確認します。

---

### **コマンドラインでの設定方法**

Visual Studio のプロジェクトファイルを直接編集せずに、コマンドラインでスタックサイズを設定することも可能です。

1. **`/STACK` オプションを使用**
   - スタックサイズを設定するには、リンカーオプション `/STACK` を使用します。
   - 例：

     ```cmd
     cl /Femain.exe main.cpp /link /STACK:18874368
     ```

2. **オプションの意味**
   - `/STACK:<reserve>[,<commit>]`
     - `<reserve>`: スタックの予約サイズ（バイト単位）。
     - `<commit>`: スタックのコミットサイズ（バイト単位、任意）。

---

### **確認方法**

設定が正しく反映されているかを確認するには、以下の方法を使用します。

1. **`dumpbin` コマンドで確認**
   - ビルドされた実行ファイルのスタックサイズを確認するには、以下のコマンドを実行します：

    ```cmd
     dumpbin /headers your_program.exe
     ```

   - 出力の中に `stack reserve size` が表示され、設定したスタックサイズ（18MB = 18874368 バイト）が確認できます。

2. **プログラムで確認**
   - 再帰関数や大きなローカル変数を使用して、スタックサイズが増加してもエラーが発生しないことを確認します。

---

### **注意事項**

- スタックサイズを大きく設定すると、システム全体のメモリ使用量に影響を与える可能性があります。必要以上に大きな値を設定しないようにしてください。
- スタックサイズを変更する必要がある場合は、通常、再帰の深さやローカル変数のサイズが大きいプログラムに限られます。

---

これで、Visual Studio プロジェクトでスタックメモリを 18MB に設定する方法が完了です！何か他に質問があれば、ぜひお知らせください 😊

## ヒープメモリの設定方法

ヒープメモリのサイズは、**システムの物理メモリ**や**仮想メモリの設定**に依存します。C++ プログラムで使用できるヒープメモリの最大サイズは、以下の要因によって決まります。

---

## **1. ヒープメモリのサイズの決定要因**

### **Windows**

- **ヒープメモリのデフォルトサイズ**
  - Windows では、プロセスごとにデフォルトのヒープサイズが設定されています。
  - デフォルトの予約サイズは **1MB** ですが、必要に応じて動的に拡張されます。
  - 最大サイズは、システムの物理メモリと仮想メモリの設定に依存します。

- **仮想メモリの制限**
  - 32ビットアプリケーションでは、プロセスごとに最大 **2GB**（または `/LARGEADDRESSAWARE` オプションを使用すると最大 **4GB**）。
  - 64ビットアプリケーションでは、プロセスごとの仮想メモリ空間は **8TB** 以上（システムの物理メモリに依存）。

---

### **Linux**

- **ヒープメモリのデフォルトサイズ**
  - Linux では、ヒープメモリのサイズはシステムの物理メモリとスワップ領域に依存します。
  - プロセスごとの仮想メモリ制限は、`ulimit` コマンドで確認できます。

- **仮想メモリの制限**
  - 32ビットアプリケーションでは、プロセスごとに最大 **3GB**（カーネルが 1GB を使用）。
  - 64ビットアプリケーションでは、プロセスごとの仮想メモリ空間は **128TB** 以上（システムの物理メモリに依存）。

---

## **2. ヒープメモリのサイズを確認する方法**

### **Windows**

1. **デフォルトのヒープサイズを確認**
   - Visual Studio でプロジェクトのプロパティを開き、以下の設定を確認します：
     **[リンカー] → [システム] → [ヒープの予約サイズ]**
   - デフォルトでは、予約サイズは **1MB**、コミットサイズは **4KB** に設定されています。

2. **`dumpbin` コマンドで確認**
   - 実行ファイルのヒープサイズを確認するには、以下のコマンドを使用します：

     ```cmd
     dumpbin /headers your_program.exe
     ```

   - 出力の中に `heap reserve size` と `heap commit size` が表示されます。

3. **プログラムで確認**
   - 動的にメモリを割り当て続けて、どこで失敗するかを確認します。
   - 例：

     ```cpp
     #include <iostream>
     #include <vector>

     int main() {
         std::vector<void*> allocations;
         size_t allocated = 0;

         try {
             while (true) {
                 void* block = malloc(1024 * 1024); // 1MB を割り当て
                 if (!block) break;
                 allocations.push_back(block);
                 allocated++;
                 std::cout << "Allocated: " << allocated << " MB" << std::endl;
             }
         } catch (...) {
             std::cout << "Heap allocation failed!" << std::endl;
         }

         for (void* block : allocations) {
             free(block); // メモリを解放
         }

         std::cout << "Total allocated: " << allocated << " MB" << std::endl;
         return 0;
     }
     ```

---

### **Linux**

1. **`ulimit` コマンドで確認**
   - プロセスごとの仮想メモリ制限を確認するには、以下のコマンドを使用します：

     ```bash
     ulimit -v
     ```

   - 出力結果が仮想メモリの最大量（KB単位）です。

2. **プログラムで確認**
   - Windows の例と同様に、動的にメモリを割り当て続けて、どこで失敗するかを確認します。

---

## **3. ヒープメモリのサイズを変更する方法**

### **Windows**

1. **Visual Studio のプロジェクト設定で変更**
   - プロジェクトのプロパティを開き、以下の設定を変更します：
     **[リンカー] → [システム] → [ヒープの予約サイズ]**
   - 例：ヒープの予約サイズを 64MB に設定する場合：
     - **ヒープの予約サイズ**: `67108864`（64MB = 64 × 1024 × 1024 バイト）
     - **ヒープのコミットサイズ**: 必要に応じて設定（通常はデフォルトのままで問題ありません）。

2. **コマンドラインで変更**
   - リンカーオプション `/HEAP` を使用してヒープサイズを変更できます。
   - 例：ヒープサイズを 64MB に設定する場合：

     ```cmd
     cl /Femain.exe main.cpp /link /HEAP:67108864
     ```

---

### **Linux**

1. **`ulimit` コマンドで変更**
   - プロセスごとの仮想メモリ制限を変更するには、以下のコマンドを使用します：

     ```bash
     ulimit -v [サイズ]
     ```

   - 例：仮想メモリ制限を 2GB に設定する場合：

     ```bash
     ulimit -v 2097152
     ```

2. **システム全体の設定を変更**
   - `/etc/security/limits.conf` ファイルを編集して、プロセスごとのメモリ制限を変更できます。

---

## **4. ヒープメモリとスタックメモリの違い**

| **項目**               | **スタックメモリ**                              | **ヒープメモリ**                              |
|------------------------|-----------------------------------------------|---------------------------------------------|
| **メモリの割り当て場所** | スタック                                      | ヒープ                                       |
| **メモリの管理**         | 自動（スコープを抜けると自動解放）              | 手動（`delete` を使って明示的に解放が必要）   |
| **速度**               | 高速                                         | 遅い                                        |
| **メモリサイズの制限**   | スタックサイズに依存（通常数 MB 程度）           | 制限が少ない（システムのメモリ容量に依存）    |
| **用途**               | 短命なデータや軽量なデータ                     | 長命なデータや動的にサイズが変わるデータ     |
| **メモリリークのリスク** | なし                                         | `delete` を忘れるとメモリリークが発生する     |

---

## **5. まとめ**

- **ヒープメモリのサイズ**
  - Windows では、デフォルトの予約サイズは 1MB ですが、必要に応じて動的に拡張されます。
  - Linux では、システムの物理メモリとスワップ領域に依存します。

- **ヒープメモリの確認方法**
  - プログラム内で動的にメモリを割り当て続けることで、ヒープメモリの限界を確認できます。
  - Windows では `dumpbin` コマンド、Linux では `ulimit` コマンドを使用して確認できます。

- **ヒープメモリの変更**
  - Windows では Visual Studio のプロジェクト設定や `/HEAP` オプションで変更可能。
  - Linux では `ulimit` コマンドやシステム設定を変更することで対応可能。

ヒープメモリのサイズは、システムのリソースに依存するため、必要に応じて適切に設定してください。

---

## Handleのメモリ管理

`Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());` のコードでは、`shapeTool` は **`new` 宣言されていません**。このコードは、`XCAFDoc_DocumentTool::ShapeTool` メソッドを呼び出して、既存の `XCAFDoc_ShapeTool` オブジェクトを取得しているだけです。

---

### **詳細な説明**

#### **1. `XCAFDoc_DocumentTool::ShapeTool` の動作**

- `XCAFDoc_DocumentTool::ShapeTool` は、`doc->Main()`（OCAF ドキュメントのルートラベル）から `XCAFDoc_ShapeTool` を取得するための静的メソッドです。
- このメソッドは、OCAF ドキュメント内に既に存在する `XCAFDoc_ShapeTool` を検索し、`Handle` を返します。
- 必要に応じて、`XCAFDoc_ShapeTool` がまだ存在しない場合は、新しいインスタンスを作成してドキュメントに登録する可能性があります（内部で `new` を使用している場合があります）。

---

#### **2. `Handle` の役割**

- `Handle` はスマートポインタであり、`XCAFDoc_ShapeTool` のような `Standard_Transient` を継承したクラスのインスタンスを管理します。
- `XCAFDoc_DocumentTool::ShapeTool` メソッドは、`Handle(XCAFDoc_ShapeTool)` を返すため、`shapeTool` はそのオブジェクトを共有する形になります。

---

#### **3. `new` の有無について**

- **コード内で `new` を明示的に呼び出していない**ため、`shapeTool` 自体は新しいオブジェクトを作成していません。
- ただし、`XCAFDoc_DocumentTool::ShapeTool` の内部で、必要に応じて `new` を使用して `XCAFDoc_ShapeTool` を作成している可能性があります。

---

### **コードの流れを簡単に説明**

以下のコードを例に、`shapeTool` がどのように取得されるかを説明します。

```cpp
// OCAF ドキュメントを作成
Handle(TDocStd_Document) doc = new TDocStd_Document("XmlOcaf");

// ShapeTool と ColorTool を取得
Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(doc->Main());
```

1. **`doc->Main()`**
   - OCAF ドキュメントのルートラベル（`TDF_Label`）を取得します。
   - このラベルは、OCAF ドキュメント内のすべてのデータ構造のエントリポイントです。

2. **`XCAFDoc_DocumentTool::ShapeTool(doc->Main())`**
   - `doc->Main()` を引数として渡し、`XCAFDoc_ShapeTool` を取得します。
   - このメソッドは、ルートラベルに関連付けられた `XCAFDoc_ShapeTool` を検索します。
   - もし `XCAFDoc_ShapeTool` がまだ存在しない場合、内部で `new` を使用して新しいインスタンスを作成し、ルートラベルに登録します。

3. **`Handle` の役割**
   - `Handle(XCAFDoc_ShapeTool)` は、`XCAFDoc_ShapeTool` オブジェクトを参照するスマートポインタです。
   - 参照カウントを管理し、オブジェクトのライフサイクルを制御します。

---

### **`XCAFDoc_DocumentTool::ShapeTool` の内部動作（推測）**

`XCAFDoc_DocumentTool::ShapeTool` の内部は、以下のような動作をしていると考えられます。

```cpp
Handle(XCAFDoc_ShapeTool) XCAFDoc_DocumentTool::ShapeTool(const TDF_Label& label) {
    // ラベルに関連付けられた ShapeTool を検索
    Handle(XCAFDoc_ShapeTool) shapeTool;
    if (!label.FindAttribute(XCAFDoc_ShapeTool::GetID(), shapeTool)) {
        // ShapeTool が存在しない場合、新しいインスタンスを作成
        shapeTool = new XCAFDoc_ShapeTool();
        label.AddAttribute(shapeTool); // ラベルに登録
    }
    return shapeTool;
}
```

- **`label.FindAttribute`**
  - ラベルに関連付けられた `XCAFDoc_ShapeTool` を検索します。
  - 見つかった場合は、それを `Handle` に格納して返します。

- **`new XCAFDoc_ShapeTool()`**
  - `XCAFDoc_ShapeTool` が見つからない場合、新しいインスタンスを作成します。

- **`label.AddAttribute`**
  - 作成した `XCAFDoc_ShapeTool` をラベルに登録します。

---

### **4. まとめ**

- **`shapeTool` はメソッド内で `new` 宣言されているか？**
  - **直接的にはされていません**。
  - ただし、`XCAFDoc_DocumentTool::ShapeTool` の内部で、必要に応じて `new` を使用して `XCAFDoc_ShapeTool` を作成している可能性があります。

- **`Handle` の役割**
  - `Handle` は、`XCAFDoc_ShapeTool` のようなオブジェクトを参照し、参照カウントを管理します。
  - `Handle` を通じて、OCAF ドキュメント内のデータ構造を安全に操作できます。

- **`XCAFDoc_DocumentTool::ShapeTool` の動作**
  - 既存の `XCAFDoc_ShapeTool` を検索し、見つからない場合は新しいインスタンスを作成して返します。

この仕組みにより、OCAF ドキュメント内のデータ構造が効率的に管理されます。

## 例

以下は、`doc` と `shapeTool` がそれぞれどのメモリ上に存在するかについての説明です。

---

### **1. `doc` のメモリ上の配置**

```cpp
Handle(TDocStd_Document) doc = new TDocStd_Document("XmlOcaf");
```

#### **`doc` の構造**

- **`doc` 自体**:
  - `doc` は `Handle(TDocStd_Document)` 型のスマートポインタです。
  - このスマートポインタは、通常の C++ オブジェクトとして **スタックメモリ** 上に存在します（このコードではローカル変数として宣言されているため）。
- **`doc` が管理するオブジェクト**:
  - `new TDocStd_Document("XmlOcaf")` によって、`TDocStd_Document` のインスタンスが **ヒープメモリ** 上に動的に割り当てられます。
  - `doc` は、このヒープメモリ上のオブジェクトを参照し、参照カウントを管理します。

#### **メモリの流れ**

1. **スタックメモリ**:
   - `Handle(TDocStd_Document) doc` はスタック上に存在します。
2. **ヒープメモリ**:
   - `TDocStd_Document` のインスタンスはヒープメモリ上に動的に割り当てられます。

---

### **2. `shapeTool` のメモリ上の配置**

```cpp
Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(doc->Main());
```

#### **`shapeTool` の構造**

- **`shapeTool` 自体**:
  - `shapeTool` は `Handle(XCAFDoc_ShapeTool)` 型のスマートポインタです。
  - このスマートポインタは、通常の C++ オブジェクトとして **スタックメモリ** 上に存在します（このコードではローカル変数として宣言されているため）。
- **`shapeTool` が管理するオブジェクト**:
  - `XCAFDoc_DocumentTool::ShapeTool(doc->Main())` によって取得される `XCAFDoc_ShapeTool` のインスタンスは、**ヒープメモリ** 上に存在します。
  - このインスタンスは、OCAF ドキュメント（`doc`）のルートラベル（`doc->Main()`）に関連付けられています。

#### **メモリの流れ**

1. **スタックメモリ**:
   - `Handle(XCAFDoc_ShapeTool) shapeTool` はスタック上に存在します。
2. **ヒープメモリ**:
   - `XCAFDoc_ShapeTool` のインスタンスは、OCAF ドキュメント内でヒープメモリ上に動的に割り当てられます。

---

### **3. `doc` と `shapeTool` の関係**

- **`doc` の役割**:
  - `doc` は OCAF ドキュメント全体を管理するスマートポインタです。
  - `doc->Main()` を通じて、OCAF ドキュメント内のルートラベル（`TDF_Label`）にアクセスします。

- **`shapeTool` の役割**:
  - `shapeTool` は、OCAF ドキュメント内の形状データ（`XCAFDoc_ShapeTool`）を管理するスマートポインタです。
  - `XCAFDoc_DocumentTool::ShapeTool(doc->Main())` を呼び出すことで、OCAF ドキュメントのルートラベルに関連付けられた `XCAFDoc_ShapeTool` を取得します。

---

### **4. メモリの配置を図で表す**

以下は、`doc` と `shapeTool` のメモリ配置を図で表したものです。

```cmd
スタックメモリ:
+---------------------------+
| Handle(TDocStd_Document)  | --> ヒープメモリ上の TDocStd_Document
| Handle(XCAFDoc_ShapeTool) | --> ヒープメモリ上の XCAFDoc_ShapeTool
+---------------------------+

ヒープメモリ:
+---------------------------+
| TDocStd_Document          | --> OCAF ドキュメント全体を管理
| XCAFDoc_ShapeTool         | --> OCAF ドキュメント内の形状データを管理
+---------------------------+
```

---

### **5. まとめ**

| **変数**       | **メモリの種類**       | **説明**                                                                 |
|----------------|-----------------------|--------------------------------------------------------------------------|
| `doc`          | スタックメモリ         | `Handle(TDocStd_Document)` 型のスマートポインタ。スタック上に存在。       |
| `doc` が管理するオブジェクト | ヒープメモリ         | `TDocStd_Document` のインスタンス。ヒープメモリ上に動的に割り当てられる。 |
| `shapeTool`    | スタックメモリ         | `Handle(XCAFDoc_ShapeTool)` 型のスマートポインタ。スタック上に存在。       |
| `shapeTool` が管理するオブジェクト | ヒープメモリ         | `XCAFDoc_ShapeTool` のインスタンス。ヒープメモリ上に動的に割り当てられる。 |

---

### **補足**

- **`Handle` の役割**:
  - `Handle` は、ヒープメモリ上のオブジェクトを安全に管理するためのスマートポインタです。
  - 参照カウントを使用して、オブジェクトのライフサイクルを自動的に制御します。

- **OCAF ドキュメントの構造**:
  - OCAF ドキュメントは、ツリー構造を持つラベル（`TDF_Label`）を通じてデータを管理します。
  - `XCAFDoc_ShapeTool` は、このツリー構造の一部として管理されます。

これで、`doc` と `shapeTool` がどのメモリ上に存在するかが明確になりました！ 😊

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
