---
title: Viewer
---

## `CreateWindowEx`

Windows API の関数で、拡張スタイルを指定してウィンドウを作成します。その引数は以下の通りです。

---

### 関数シグネチャ

```cpp
HWND CreateWindowEx(
    DWORD     dwExStyle,       // 拡張ウィンドウスタイル
    LPCWSTR   lpClassName,     // ウィンドウクラス名
    LPCWSTR   lpWindowName,    // ウィンドウのタイトル
    DWORD     dwStyle,         // ウィンドウスタイル
    int       X,               // ウィンドウの初期 x 座標
    int       Y,               // ウィンドウの初期 y 座標
    int       nWidth,          // ウィンドウの幅
    int       nHeight,         // ウィンドウの高さ
    HWND      hWndParent,      // 親ウィンドウのハンドル
    HMENU     hMenu,           // メニューのハンドル
    HINSTANCE hInstance,       // インスタンスハンドル
    LPVOID    lpParam          // ウィンドウ作成データ
);
```

---

### 引数の説明

1. **`dwExStyle`**  
   - ウィンドウの拡張スタイルを指定します。
   - 例: `WS_EX_OVERLAPPEDWINDOW`, `WS_EX_TOPMOST`, `WS_EX_TOOLWINDOW` など。

2. **`lpClassName`**  
   - ウィンドウクラス名を指定します。
   - `RegisterClass` または `RegisterClassEx` で登録したクラス名を指定します。

3. **`lpWindowName`**  
   - ウィンドウのタイトルバーに表示される文字列を指定します。

4. **`dwStyle`**  
   - ウィンドウのスタイルを指定します。
   - 例: `WS_OVERLAPPEDWINDOW`, `WS_POPUP`, `WS_CHILD` など。

5. **`X`**  
   - ウィンドウの左上隅の x 座標を指定します。
   - `CW_USEDEFAULT` を指定すると、システムがデフォルトの位置を決定します。

6. **`Y`**  
   - ウィンドウの左上隅の y 座標を指定します。
   - `CW_USEDEFAULT` を指定すると、システムがデフォルトの位置を決定します。

7. **`nWidth`**  
   - ウィンドウの幅をピクセル単位で指定します。

8. **`nHeight`**  
   - ウィンドウの高さをピクセル単位で指定します。

9. **`hWndParent`**  
   - 親ウィンドウのハンドルを指定します。
   - 親ウィンドウがない場合は `nullptr` を指定します。

10. **`hMenu`**  
    - メニューのハンドルを指定します。
    - メニューがない場合は `nullptr` を指定します。

11. **`hInstance`**  
    - アプリケーションのインスタンスハンドルを指定します。

12. **`lpParam`**  
    - ウィンドウ作成データへのポインタを指定します。
    - 通常は `nullptr` を指定します。

---

### 戻り値

- 成功時: 作成されたウィンドウのハンドル (`HWND`) を返します。
- 失敗時: `nullptr` を返します。

### 注意点

- `CreateWindowEx` を呼び出す前に、`RegisterClass` または `RegisterClassEx` を使用してウィンドウクラスを登録する必要があります。
- 作成したウィンドウを表示するには、`ShowWindow` と `UpdateWindow` を呼び出します。

これで、`CreateWindowEx` の引数について理解できるはずです。
