---
title: FairCurve
---

以下に、`FairCurve_EnergyOfMVC.hxx` と FairCurve_EnergyOfBatten.hxx のヘッダファイル内で定義されている計算やメソッドについて、それぞれの役割と動作を説明します。

---

## **1. FairCurve_EnergyOfMVC.hxx**

このクラスは、最小変動曲線（Minimal Variation Curve, MVC）のエネルギー基準を計算し、最適化するためのクラスです。

### **(1) コンストラクタ**

```cpp
FairCurve_EnergyOfMVC(
    const Standard_Integer BSplOrder,
    const Handle(TColStd_HArray1OfReal)& FlatKnots,
    const Handle(TColgp_HArray1OfPnt2d)& Poles,
    const Standard_Integer ContrOrder1,
    const Standard_Integer ContrOrder2,
    const FairCurve_BattenLaw& Law,
    const Standard_Real PhysicalRatio,
    const Standard_Real LengthSliding,
    const Standard_Boolean FreeSliding = Standard_True,
    const Standard_Real Angle1 = 0,
    const Standard_Real Angle2 = 0,
    const Standard_Real Curvature1 = 0,
    const Standard_Real Curvature2 = 0);
```

- **役割**:
  - Bスプライン曲線のエネルギー最小化を行うための初期化を行います。
  - 曲線の次数、ノットベクトル、制御点、制約条件（角度や曲率など）を設定します。

- **引数の説明**:
  - `BSplOrder`: Bスプライン曲線の次数。
  - `FlatKnots`: ノットベクトル。
  - `Poles`: 制御点（2D座標）。
  - `ContrOrder1`, `ContrOrder2`: 制約条件の次数。
  - `Law`: バテン法則（FairCurve_BattenLaw）で、曲線の形状を制御するための法則。
  - `PhysicalRatio`: 曲線の物理的な比率。
  - `LengthSliding`: 曲線のスライディング長さ。
  - `FreeSliding`: スライディングの自由度を指定。
  - `Angle1`, `Angle2`: 曲線の端点での角度制約。
  - `Curvature1`, `Curvature2`: 曲線の端点での曲率制約。

---

### **(2) `LengthSliding` メソッド**

```cpp
Standard_Real LengthSliding() const;
```

- **役割**:
  - 曲線のスライディング長さを計算して返します。
  - スライディング長さは、曲線の端点間の距離にスライディング量を加えたものです。

---

### **(3) `Status` メソッド**

```cpp
FairCurve_AnalysisCode Status() const;
```

- **役割**:
  - 計算の状態を返します。
  - 状態コード（`FairCurve_AnalysisCode`）は、計算が成功したかどうか、またはエラーが発生したかを示します。

---

### **(4) `Variable` メソッド**

```cpp
Standard_Boolean Variable(math_Vector& X) const Standard_OVERRIDE;
```

- **役割**:
  - 最適化された制御点の座標（変数）を計算して返します。
  - `X` に最適化された制御点の座標が格納されます。

---

### **(5) `ComputePoles` メソッド**

```cpp
virtual void ComputePoles(const math_Vector& X) Standard_OVERRIDE;
```

- **役割**:
  - 与えられた変数 `X` に基づいて制御点を計算します。
  - 内部的に `Poles` を更新します。

---

### **(6) `Compute` メソッド**

```cpp
virtual Standard_Boolean Compute(const Standard_Integer DerivativeOrder, math_Vector& Result) Standard_OVERRIDE;
```

- **役割**:
  - エネルギーを計算します。
  - `DerivativeOrder` に基づいて、エネルギーの値やその導関数を計算します。
  - 計算結果を `Result` に格納します。

---

## **2. FairCurve_EnergyOfBatten.hxx**

このクラスは、バテン法則（Batten Law）に基づいて曲線のエネルギーを計算し、最適化するためのクラスです。

### **(1) コンストラクタ**

```cpp
FairCurve_EnergyOfBatten(
    const Standard_Integer BSplOrder,
    const Handle(TColStd_HArray1OfReal)& FlatKnots,
    const Handle(TColgp_HArray1OfPnt2d)& Poles,
    const Standard_Integer ContrOrder1,
    const Standard_Integer ContrOrder2,
    const FairCurve_BattenLaw& Law,
    const Standard_Real LengthSliding,
    const Standard_Boolean FreeSliding = Standard_True,
    const Standard_Real Angle1 = 0,
    const Standard_Real Angle2 = 0);
```

- **役割**:
  - バテン法則に基づいて曲線のエネルギー最小化を行うための初期化を行います。
  - 曲線の次数、ノットベクトル、制御点、制約条件（角度など）を設定します。

- **引数の説明**:
  - `BSplOrder`: Bスプライン曲線の次数。
  - `FlatKnots`: ノットベクトル。
  - `Poles`: 制御点（2D座標）。
  - `ContrOrder1`, `ContrOrder2`: 制約条件の次数。
  - `Law`: バテン法則（FairCurve_BattenLaw）で、曲線の形状を制御するための法則。
  - `LengthSliding`: 曲線のスライディング長さ。
  - `FreeSliding`: スライディングの自由度を指定。
  - `Angle1`, `Angle2`: 曲線の端点での角度制約。

---

### **(2) `LengthSliding` メソッド**

```cpp
Standard_Real LengthSliding() const;
```

- **役割**:
  - 曲線のスライディング長さを計算して返します。
  - スライディング長さは、曲線の端点間の距離にスライディング量を加えたものです。

---

### **(3) `Status` メソッド**

```cpp
FairCurve_AnalysisCode Status() const;
```

- **役割**:
  - 計算の状態を返します。
  - 状態コード（`FairCurve_AnalysisCode`）は、計算が成功したかどうか、またはエラーが発生したかを示します。

---

### **(4) `Variable` メソッド**

```cpp
Standard_Boolean Variable(math_Vector& X) const Standard_OVERRIDE;
```

- **役割**:
  - 最適化された制御点の座標（変数）を計算して返します。
  - `X` に最適化された制御点の座標が格納されます。

---

### **(5) `ComputePoles` メソッド**

```cpp
virtual void ComputePoles(const math_Vector& X) Standard_OVERRIDE;
```

- **役割**:
  - 与えられた変数 `X` に基づいて制御点を計算します。
  - 内部的に `Poles` を更新します。

---

### **(6) `Compute` メソッド**

```cpp
virtual Standard_Boolean Compute(const Standard_Integer DerivativeOrder, math_Vector& Result) Standard_OVERRIDE;
```

- **役割**:
  - エネルギーを計算します。
  - `DerivativeOrder` に基づいて、エネルギーの値やその導関数を計算します。
  - 計算結果を `Result` に格納します。

---

## **3. 共通点と違い**

- **共通点**:
  - 両クラスとも、エネルギー最小化を行い、最適化された制御点を計算します。
  - `LengthSliding`、`Status`、`Variable`、`ComputePoles` などのメソッドを持ちます。

- **違い**:
  - `FairCurve_EnergyOfMVC` は最小変動曲線（MVC）に基づくエネルギー計算を行います。
  - `FairCurve_EnergyOfBatten` はバテン法則に基づくエネルギー計算を行います。

---

これらのクラスは、曲線の形状を最適化するための強力なツールであり、エネルギー基準に基づいて滑らかで制約条件を満たす曲線を生成するのに役立ちます。

Similar code found with 1 license type

### **バテン法則（Batten Law）とは？**

バテン法則（Batten Law）は、曲線の形状を制御するための物理的または数学的な法則です。OpenCASCADE の `FairCurve_BattenLaw` クラスでは、バテン法則を用いて曲線のエネルギーを計算し、滑らかで制約条件を満たす曲線を生成します。

---

### **1. バテン法則の背景**

バテン法則は、物理的なバテン（薄い板や棒）の挙動をモデル化したもので、以下のような特性を持つ曲線を生成するために使用されます：

- 曲線が滑らかである（急激な曲率変化がない）。
- 曲線が物理的な制約条件（長さ、角度、曲率など）を満たす。
- 曲線がエネルギー的に最適化されている（最小エネルギー状態）。

---

### **2. バテン法則のエネルギー定義**

バテン法則に基づくエネルギーは、以下のような要素を考慮して定義されます：

#### **(1) 曲線の滑らかさ**

曲線の滑らかさを評価するために、曲線の2次導関数（曲率）や3次導関数（ねじれ）を基準にエネルギーを計算します。

\[
E_{\text{smoothness}} = \int_a^b \left( \frac{d^2 C(t)}{dt^2} \right)^2 dt
\]

ここで：

- \( C(t) \) は曲線のパラメータ化された表現。
- \( \frac{d^2 C(t)}{dt^2} \) は曲線の2次導関数（曲率）。

#### **(2) 曲線の長さ**

曲線の全体的な長さを最小化するためのエネルギー項です。

\[
E_{\text{length}} = \int_a^b \left| \frac{dC(t)}{dt} \right| dt
\]

#### **(3) 制約条件**

曲線の端点での角度や曲率などの制約条件を満たすためのエネルギー項です。

\[
E_{\text{constraints}} = \lambda_1 \left( \text{角度の誤差} \right)^2 + \lambda_2 \left( \text{曲率の誤差} \right)^2
\]

ここで：

- \(\lambda_1, \lambda_2\) は制約条件の重み。

---

### **3. `FairCurve_BattenLaw` クラスの役割**

OpenCASCADE の `FairCurve_BattenLaw` クラスは、バテン法則を用いて曲線の形状を制御します。このクラスは、以下のような役割を持ちます：

#### **(1) 曲線のエネルギー計算**

バテン法則に基づいて、曲線のエネルギーを計算します。

#### **(2) 曲線の形状制御**

曲線の滑らかさや制約条件を満たすように、制御点の座標を調整します。

#### **(3) 制約条件の適用**

曲線の端点での角度や曲率などの制約条件を適用します。

---

### **4. `FairCurve_BattenLaw` の使用例**

以下は、`FairCurve_BattenLaw` を使用して曲線の形状を制御する例です：

```cpp
// バテン法則を定義
Standard_Real Heigth = 1.0;  // 中央点の高さ
Standard_Real Slope = 0.5;   // 幾何学的な傾斜
Standard_Real Sliding = 2.0; // 有効長さ

FairCurve_BattenLaw battenLaw(Heigth, Slope, Sliding);

// Bスプライン曲線のエネルギー最小化に使用
FairCurve_EnergyOfBatten energyOfBatten(
    BSplOrder,
    flatKnots,
    poles2d,
    ContrOrder1,
    ContrOrder2,
    battenLaw,
    LengthSliding,
    FreeSliding,
    Angle1,
    Angle2);
```

---

### **5. バテン法則のパラメータ**

`FairCurve_BattenLaw` クラスでは、以下のパラメータを使用してバテン法則を定義します：

#### **(1) `Heigth`**

- 中央点の高さを指定します。
- 曲線の形状を制御するための重要なパラメータです。

#### **(2) `Slope`**

- 曲線の傾斜を指定します。
- 曲線の滑らかさや形状に影響を与えます。

#### **(3) `Sliding`**

- 曲線の有効長さを指定します。
- 曲線の端点間の距離に影響を与えます。

---

### **6. バテン法則の応用例**

バテン法則は、以下のような応用に使用されます：

- **CAD/CAM**:
  - 滑らかな曲線やサーフェスの生成。
- **建築設計**:
  - 建築物の曲線的なデザイン。
- **航空宇宙工学**:
  - 空力的に最適化された形状の設計。

---

### **7. 結論**

バテン法則は、曲線の形状を制御するための強力なツールであり、滑らかで制約条件を満たす曲線を生成するのに役立ちます。OpenCASCADE の `FairCurve_BattenLaw` クラスは、この法則を実装しており、エネルギー最小化を通じて効率的な曲線設計を可能にします。

これは OpenCASCADE の `FairCurve_MinimalVariation` クラスのヘッダーファイルです。このクラスは、2D 曲線を計算するためのアルゴリズムを提供し、曲線の「ねじれ（Jerk）」や「たるみ（Sagging）」エネルギーを最小化することで、滑らかで制約条件を満たす曲線を生成します。

---

## **`FairCurve_MinimalVariation` クラスの概要**

`FairCurve_MinimalVariation` クラスは、以下の目的で使用されます：

1. **エネルギー最小化**:
   - 曲線の「ねじれエネルギー」や「たるみエネルギー」を最小化します。
2. **制約条件の適用**:
   - 曲線の端点での角度や曲率などの制約条件を適用します。
3. **滑らかな曲線の生成**:
   - 曲線の滑らかさを最適化し、物理的に自然な形状を生成します。

---

### **主な特徴**

#### **1. エネルギーの種類**

このクラスでは、以下の2種類のエネルギーを最小化できます：

- **ねじれエネルギー（Jerk Energy）**:
  - 曲線の3次導関数（ねじれ）を基準にエネルギーを計算します。
  - 滑らかさを重視した曲線を生成します。
- **たるみエネルギー（Sagging Energy）**:
  - 曲線の形状がたるむ（柔軟に曲がる）ようにエネルギーを計算します。

#### **2. 制約条件**

- 曲線の端点での角度や曲率を指定できます。
- これにより、特定の形状や物理的条件を満たす曲線を生成できます。

---

### **主なメソッドとその役割**

#### **1. コンストラクタ**

```cpp
FairCurve_MinimalVariation(const gp_Pnt2d& P1, const gp_Pnt2d& P2, const Standard_Real Heigth, const Standard_Real Slope = 0, const Standard_Real PhysicalRatio = 0);
```

- **役割**:
  - 曲線の端点（`P1` と `P2`）を定義し、バテン（弾性ビーム）の幾何学的特性を初期化します。
  - 曲線の高さ（`Heigth`）、傾斜（`Slope`）、エネルギーの種類（`PhysicalRatio`）を設定します。

- **引数の説明**:
  - `P1`, `P2`: 曲線の端点（2D座標）。
  - `Heigth`: 曲線の変形の高さ。
  - `Slope`: 曲線の傾斜。
  - `PhysicalRatio`: エネルギーの種類（0: ねじれエネルギー、1: たるみエネルギー）。

---

#### **2. `SetCurvature1` と `SetCurvature2`**

```cpp
void SetCurvature1(const Standard_Real Curvature);
void SetCurvature2(const Standard_Real Curvature);
```

- **役割**:
  - 曲線の端点（`P1` と `P2`）での曲率を設定します。

---

#### **3. `SetPhysicalRatio`**

```cpp
void SetPhysicalRatio(const Standard_Real Ratio);
```

- **役割**:
  - エネルギーの種類を設定します。
  - `Ratio = 0` の場合は「ねじれエネルギー」のみを使用。
  - `Ratio = 1` の場合は「たるみエネルギー」のみを使用。

- **注意点**:
  - `Ratio = 1` の場合、曲率の制約を設定することはできません。

---

#### **4. `Compute`**

```cpp
Standard_Boolean Compute(FairCurve_AnalysisCode& ACode, const Standard_Integer NbIterations = 50, const Standard_Real Tolerance = 1.0e-3);
```

- **役割**:
  - 曲線を計算します。
  - 制約条件（角度、曲率など）を考慮しながら、指定された反復回数（`NbIterations`）と許容誤差（`Tolerance`）で計算を行います。

- **戻り値**:
  - 計算が成功した場合は `Standard_True` を返します。

---

#### **5. `GetCurvature1` と `GetCurvature2`**

```cpp
Standard_Real GetCurvature1() const;
Standard_Real GetCurvature2() const;
```

- **役割**:
  - 曲線の端点（`P1` と `P2`）での曲率を取得します。

---

#### **6. `GetPhysicalRatio`**

```cpp
Standard_Real GetPhysicalRatio() const;
```

- **役割**:
  - 現在設定されているエネルギーの種類（物理的比率）を取得します。

---

#### **7. `Dump`**

```cpp
virtual void Dump(Standard_OStream& o) const Standard_OVERRIDE;
```

- **役割**:
  - 現在のオブジェクトの状態を指定されたストリーム（`o`）に出力します。

---

### **使用例**

以下は、`FairCurve_MinimalVariation` を使用して曲線を計算する例です：

```cpp
// 曲線の端点を定義
gp_Pnt2d P1(0.0, 0.0);
gp_Pnt2d P2(1.0, 1.0);

// バテン法則のパラメータを設定
Standard_Real Heigth = 1.0;
Standard_Real Slope = 0.5;
Standard_Real PhysicalRatio = 0.0; // ねじれエネルギーを使用

// FairCurve_MinimalVariation を初期化
FairCurve_MinimalVariation minimalVariation(P1, P2, Heigth, Slope, PhysicalRatio);

// 曲率を設定
minimalVariation.SetCurvature1(0.1);
minimalVariation.SetCurvature2(0.2);

// 曲線を計算
FairCurve_AnalysisCode status;
if (minimalVariation.Compute(status)) {
    std::cout << "Curve computed successfully." << std::endl;
} else {
    std::cerr << "Failed to compute curve." << std::endl;
}
```

---

### **結論**

`FairCurve_MinimalVariation` クラスは、曲線の「ねじれエネルギー」や「たるみエネルギー」を最小化し、滑らかで制約条件を満たす曲線を生成するための強力なツールです。このクラスは、CAD/CAM、建築設計、航空宇宙工学など、さまざまな分野での曲線設計に役立ちます。

## OpenCASCADEの`FairCurve_*`クラス群

OpenCASCADEの`FairCurve_*`クラス群（例: `FairCurve_EnergyOfMVC`, `FairCurve_EnergyOfBatten`, `FairCurve_MinimalVariation`など）は、数学的に基づいたエネルギー最小化アルゴリズムを使用しており、CAD/CAMや工業設計において滑らかな曲線を生成するための強力なツールです。その妥当性については、以下の観点から評価できます。

---

### **1. 数学的基盤**

- **エネルギー最小化**:
  - `FairCurve_*`クラスは、曲線の滑らかさや形状を最適化するために、エネルギー最小化の原理に基づいています。
  - 具体的には、曲線の「ねじれエネルギー」や「たるみエネルギー」を最小化することで、滑らかで自然な形状を生成します。
  - 数学的には、これらのエネルギーは曲線の導関数（2次導関数や3次導関数）を用いて定義されており、物理的なバテン（薄い板や棒）の挙動をモデル化しています。

- **制約条件**:
  - 曲線の端点での角度や曲率などの制約条件を適用できるため、特定の設計要件を満たす曲線を生成できます。

- **ニュートン法や最適化アルゴリズム**:
  - `FairCurve_Newton`などのクラスを使用して、エネルギー最小化問題を効率的に解くための数値計算アルゴリズムが実装されています。

**妥当性の評価**:

- 数学的に正当な基盤に基づいており、理論的には妥当性が高いといえます。

---

### **2. 実装の信頼性**

- **OpenCASCADEの実績**:
  - OpenCASCADEは、CAD/CAMや工業設計分野で広く使用されているオープンソースの幾何学モデリングライブラリです。
  - `FairCurve_*`クラスは、OpenCASCADEの一部として長年にわたり使用されており、実際の設計プロジェクトでの使用実績があります。

- **テストと検証**:
  - OpenCASCADEのライブラリは、広範なテストと実際の使用例を通じて検証されています。
  - ただし、特定の使用条件や入力データによっては、結果が期待通りでない場合もあるため、適切な入力データの準備が重要です。

**妥当性の評価**:

- 実装は信頼性が高いと考えられますが、特定の条件下での動作を確認する必要があります。

---

### **3. 使用上の制限**

- **入力データの品質**:
  - `FairCurve_*`クラスは、入力データ（制御点、ノットベクトル、制約条件など）の品質に依存します。
  - 不適切な入力データ（例: ノットベクトルが不正、制御点が不適切など）を与えると、計算が失敗したり、期待通りの結果が得られない場合があります。

- **計算コスト**:
  - エネルギー最小化アルゴリズムは計算コストが高くなる場合があります。特に、制御点の数が多い場合や複雑な制約条件を適用する場合は、計算時間が増加する可能性があります。

- **特定の用途への適合性**:
  - `FairCurve_*`クラスは、滑らかな曲線を生成するために設計されていますが、すべての用途に適しているわけではありません。
  - 例えば、非常に複雑な形状や非標準的な制約条件を扱う場合には、別のアプローチが必要になることがあります。

**妥当性の評価**:

- 入力データの品質や計算コストに注意する必要がありますが、適切に使用すれば妥当性は高いといえます。

---

### **4. 実際の使用例**

- **CAD/CAM**:
  - 滑らかな曲線やサーフェスの生成に使用されます。
  - 例: 自動車や航空機の外装設計。

- **建築設計**:
  - 建築物の曲線的なデザインに使用されます。
  - 例: 橋梁や屋根の設計。

- **航空宇宙工学**:
  - 空力的に最適化された形状の設計に使用されます。
  - 例: 翼の断面形状の設計。

**妥当性の評価**:

- 実際の設計プロジェクトでの使用実績があるため、実用的な妥当性は高いといえます。

---

### **5. 結論**

- **数学的妥当性**:
  - `FairCurve_*`クラスは、数学的に正当なエネルギー最小化アルゴリズムに基づいており、理論的な妥当性は高いです。

- **実装の信頼性**:
  - OpenCASCADEの一部として広く使用されており、実装の信頼性も高いです。

- **使用上の注意**:
  - 入力データの品質や計算コストに注意する必要があります。
  - 特定の用途に適しているかどうかを事前に確認することが重要です。

全体として、`FairCurve_*`クラスは、滑らかな曲線を生成するための強力で妥当性の高いツールであるといえます。ただし、特定の条件下での動作を確認しながら使用することが推奨されます。
