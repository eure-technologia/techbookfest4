= Go言語数値計算ことはじめ、あるいはなぜGo言語で数値計算が流行らないのか

== はじめに
はじめましてこんにちは。ここ2年くらいはもっぱらGo言語でサーバサイドAPIを作り続けている@yu81です。時々機械学習系のこともやったりしています。

== 数値計算とは
T.B.D

== Goにおける数値計算の現状
T.B.D

=== 言語仕様・標準ライブラリ
T.B.D

=== 既存ライブラリ

==== Gonum

@<href>{http://www.gonum.org/,Gonum(http://www.gonum.org/)}

T.B.D

=== 他言語との比較

== Goでスクラッチで書いてみる(とりあえず高校数学レベル)

=== 複素数
簡単に複素数の復習をしておきましょう。まず、普通の数は2乗すると必ず正の値になりますが、そうならない数を定義します。すなわち、

//texequation{
i^{2} = -1 \\
//}
//texequation{
\Rightarrow i = \sqrt(-1) (正の平方根を採用)
//}

を満たす @<b>{i} を虚数単位として定義します。

実数 a, b を用いて、複素数 @<b>{z} は次のように書けます。


//texequation{
  z = a + bi
//}

aを実部、bを虚部と呼びます。

また、実部と虚部をそれぞれ2次元の平面の横軸と縦軸にプロットした複素平面を定義し、その原点からの距離と原点から引いた線分とのなす角度を用いて表すこともあります。次のように書けます。

//texequation{
z = r(\cos(\theta)+i\sin(θ))
//}


Go言語では最新バージョンの1.10現在、複素数は以下2つの組み込み型として利用可能です。

 * complex64 （float32が実部と虚部で2つ）
 * complex128 （float64が実部と虚部で2つ）

基本的な操作は次のように書けます。

//list[go-complex-def][Go言語における組み込み複素数型の基本的な利用]{
  package main

  import (
  	"fmt"
  	"math"
  	"math/cmplx"
  )

  func main() {
  	c1 := 1 + 1i
  	c2 := 3i

  	// 四則演算
  	fmt.Printf("%#+v\n", c1+c2)
  	fmt.Printf("%#+v\n", c1-c2)
  	fmt.Printf("%#+v\n", c1*c2)
  	fmt.Printf("%#+v\n", c1/c2)

  	// 絶対値
  	fmt.Printf("%#+v\n", cmplx.Abs(c1)) // sqrt(2)
  	fmt.Printf("%#+v\n", cmplx.Abs(c2)) // 3

  	// 極形式(z=r(cos(θ)+i*sin(θ))) の係数
  	c1r, c1theta := cmplx.Polar(c1)
  	c2r, c2theta := cmplx.Polar(c2)

    // r = sqrt(2), theta = pi / 4
  	fmt.Printf("abs: %#+v, angle: %#+v\n", c1r, c1theta)
    // r = 3,       theta = pi / 2
  	fmt.Printf("abs: %#+v, angle: %#+v\n", c2r, c2theta)

  	// 極形式の係数から複素数を生成
  	c3 := cmplx.Rect(10, (5.0/8.0)*math.Pi)
  	fmt.Printf("%#+v\n", c3)

  	// ド・モアブルの公式
  	rotated := c1 * c2
  	fmt.Printf("%#+v\n", rotated) // -3+3i, r = 3 * sqrt(2), theta = (3/4) * pi
  }

//}

だいたい普通の数みたいに扱えますね！もう何も怖くない。

ここまで書いておいておきながらいうのもなんですが、今のところ通常のGo言語アプリケーション開発で複素数を使用している例はほぼないと思います。@<br>{}
それこそ、ここで言及しようとしている数値計算の一部で使われることがある程度、でしょう。@<br>{}
あるいは、数学的な性質上簡易な回転・拡大・縮小の計算に用いることもできますので、その用途も考えられますが、寡聞にして見つけたことはありません。@<br>{}
そしてそもそも、Go言語での数値計算事例はあまり存在しません。今後広範に利用されうるとすれば、機械学習でよく用いられるTensorFlowのGo言語実装で使われる可能性があるかというくらいでしょうか。

=== ベクトル
T.B.D

=== 行列
T.B.D

=== ニュートン法
T.B.D

== 実際に書いてみてから、再び他言語との比較
T.B.D

== まとめ
T.B.D

== 参考文献

 * NUMERICAL RECIPES in C[日本語版]
 * @<href>{https://ja.wikibooks.org/wiki/高等学校数学II,WikiBooks 高等学校数学II}
