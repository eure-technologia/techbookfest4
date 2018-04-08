= Go言語数値計算ことはじめ、あるいはなぜGo言語で数値計算が流行らないのか

== はじめに
はじめましてこんにちは。ここ2年くらいはもっぱらGo言語でサーバサイドAPIを作り続けている@yu81です。時々機械学習系のこともやったりしています。
今回は、Go言語における数値計算を取り巻く状況について現状の調査してみたので、そのまとめを書きたいと思います。

== 数値計算とは
数値計算といって、イメージが湧く人と湧かない人がいると思います。情報科学系の人よりも、他の理工系の自然科学系の大学に行っていた人の方が身近な概念かもしれません。
辞書的な定義をWikipedia先生に譲り、その引用を以下に記します。

引用ここから
//quote{
  数値解析（すうちかいせき、Numerical Analysis）は、数学および物理学の一分野で、代数的な方法で解を得ることが不可能な解析学上の問題を（通常は有限精度の）数値を用いて近似的に解く手法に関する学問である。

  史上最初の数学的記述の一つとして、バビロニアの粘土板 YBC 7289 を挙げることができる。これは六十進法で √2 （単位矩形の対角線の長さ）を数値的に近似したものである。三角形の辺の長さを計算できること（そして、平方根を計算できること）は、特に建築において重要である。例えば、縦横それぞれ2メートルの正方形の壁の対角線は √8 ≈ 2.83 メートルとなる。

  数値解析は、このような実用的計算の長い伝統に続くものである。バビロニアの √2 の近似のように、現代の数値解析も厳密な解を求めようとするものではない。何故なら、厳密な解を有限時間で求めることは不可能だからである。その代わりに、数値解析の多くは、ある程度の誤差の範囲内の近似解を求めようとする。

  数値解析は自然科学および工学のあらゆる分野に応用がある。計算言語学や社会統計学のように、人文科学や社会科学でも重要である。

  コンピュータ以前は、数値的な手法は数表と、大きな計算になると機械式計算機による補助も近代には使われたものの、紙とペンによる膨大な途中経過をダブルチェックして行われる大変な作業であった。（個々の計算はそろばんにより行われるかもしれないが、そろばんで「膨大な途中経過」は扱えない。計算尺も用途によっては有用だが、有効数字が限られており、それでは間に合わない分野も多い）

  コンピュータの発達以降は、従来から見るとその桁外れな計算力により応用分野が大きく広がったとともに、コンピュータ以前からのノウハウに加え、コンピュータによる計算の特性を正しく理解して使うことも重要になった。

  常微分方程式は天体（惑星、恒星、小宇宙）の軌道の計算に登場する。資産管理には最適化が利用されている。数値線型代数はデータ解析に不可欠である。確率微分方程式とマルコフ連鎖は、医学や生物学における生体細胞のシミュレーションの基本である。
//}
引用ここまで

代数的な方法で解を得ることが不可能というのは、誤解を恐れず身近な感覚で例を挙げると、紙とペンを使って時間を掛けさえすれば解けるような方程式ではない、というところでしょうか。
もちろん数値計算は代数的に解ける問題に対しても適用可能ですが、独自の価値があるのはそうでない問題に対して、ということですね。
そして、学校教育内で受けるテストに出てくる問題は、紙とペンとちょっとの時間があれば解けるように設定されていますが、現実にあるほとんどの方程式はこれが不可能なので、数値計算が本質的な手法となる、ということでしょう。

自然科学、例えば物理学において、ある現象を理解するためには様々な要素を微分的に考え、微分方程式を立ててそれを解くことでその現象に対する知見を得るという流れがあります。
次に示す流体力学の基礎方程式であるナヴィエ・ストークス方程式は、そのままでは解を求めることが不可能であるため、条件に応じた適切な近似系を導出して解いたり、数値計算で解いて利用します。

//texequation{
(\vec{v} ・ \nabla) \vec{v} = \mathrm{grad}(\frac{1}{2}\vec{v}^{2}) - \vec{v} \times \vec{\omega}
//}


== Goにおける数値計算の現状
さて、Go言語における数値計算ですが、事例があまりないといってよいでしょう。
ポインタの概念はあるものの、C言語のそれと異なり使い方を間違えるとメモリ破壊やセグメンテーションフォールトの原因となるようなものではなく、言語仕様もコンパクトで身につけやすく、処理によっては普通に書いてもC++に比肩する速度を出せるため、プログラミングを本業としない科学者の利用には非常に適していると思うのですが。


=== 言語仕様・標準ライブラリ
Goの言語仕様や標準ライブラリについて、数値計算にとって直接的に有用なものはさほどありません。ですが、そのシンプルな仕様ゆえに、計算という本分に集中出来るといえるでしょう。
組み込み型に複素数型がありますが、自然科学で直ちに利用可能なものはこれくらいでしょうか。

=== 既存ライブラリ
==== Gonum

@<href>{http://www.gonum.org/,Gonum(http://www.gonum.org/)} というものがあります。内部的にはBLAS（Basic Linear Algebra Subprograms）とLAPACK（Linear Algebra PACKage）という、わりと昔から存在するベクトルや行列、線形代数でよく用いる演算の共通ルーチン集のようなライブラリの、インターフェースを維持したGo版実装となっています。
元の実装がC言語屋FORTRANであるため、歴史を感じる作りになっています。

=== 他言語との比較
2018年現在において、数値計算に関してPythonに言及することは避けられないでしょう。
機械学習でよく用いられるPythonですが、様々な計算においてNunPyやSciPyが用いられています。非常に高度なことばかり行っていて常人には理解できない計算ばかりという先入観を持ってしまうのですが、例えばニューラルネットワークの計算も大雑把にいってしまえば座標変換と活性化関数の作用を大量に繰り返しているだけなので、高等数学といっても大学1,2年生なら理解できるレベルではあるはずです。
機械学習の盛り上がりもあり、現在数値計算で最も用いられている言語といえばPythonといっても過言ではなく、NumPyが最も用いられている数値計算ライブラリというのもまた然り、というところでしょうか。
NumPyの内部実装のコア部分はC言語やFORTRANによって実装されており、NumPyのインターフェースはこれらの高級なラッパーとして振る舞うため、実際の動作は通常のPythonを用いた数値計算に比べると極めて高速に動作します。

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


//list[go-vector-def][Goでベクトルの基本的な演算を実装]{
  package main

import (
	"fmt"
	"math"
)

type Vector struct {
	elements []float64
}

func NewVector(elements []float64) *Vector {
	return &Vector{elements: elements}
}

func (v *Vector) Elements() []float64 {
	return v.elements
}

func (v *Vector) Dimension() int {
	return len(v.elements)
}

func (v *Vector) Norm() float64 {
	norm := 0.0
	for i := 0; i < v.Dimension(); i++ {
		norm += v.elements[i] * v.elements[i]
	}
	return math.Sqrt(norm)
}

func Angle(v1, v2 *Vector) *float64 {
	if !hasSameDimension(v1, v2) {
		return nil
	}
	innerProduct := InnerProduct(v1, v2)
	cosine := *innerProduct / (v1.Norm() * v2.Norm())
	angle := math.Acos(cosine)
	return &angle
}

func AngleInDegree(v1, v2 *Vector) *float64 {
	if !hasSameDimension(v1, v2) {
		return nil
	}
	angleInRadian := *Angle(v1, v2)
	angleInDegree := angleInRadian * 180.0 / math.Pi
	return &angleInDegree

}

func InnerProduct(v1, v2 *Vector) *float64 {
	if !hasSameDimension(v1, v2) {
		return nil
	}
	product := 0.0
	for i := 0; i < v1.Dimension(); i++ {
		product += v1.elements[i] * v2.elements[i]
	}
	return &product
}

func hasSameDimension(v1, v2 *Vector) bool {
	if v1 == nil || v2 == nil {
		return false
	}
	return v1.Dimension() == v2.Dimension()
}

func main() {
	v1 := NewVector([]float64{1.0, 2.0})
	v2 := NewVector([]float64{4.0, 5.0})

	fmt.Println(*InnerProduct(v1, v2))

	v3 := NewVector([]float64{1.0, 0.0})
	v4 := NewVector([]float64{0.0, 1.0})
	fmt.Println(*Angle(v3, v4))
	fmt.Println(*AngleInDegree(v3, v4))
	fmt.Println(*InnerProduct(v3, v4))

	v5 := NewVector([]float64{math.Cos(math.Pi / 3), math.Sin(math.Pi / 3)})
	fmt.Println(*Angle(v3, v5))
	fmt.Println(*AngleInDegree(v3, v5))
	fmt.Println(*InnerProduct(v3, v5))
}
//}

=== 行列
T.B.D

=== ニュートン法
T.B.D

== 実際に書いてみてから、再び他言語との比較
T.B.D

== まとめ
T.B.D

== 参考文献

 * @<href>{https://ja.wikipedia.org/wiki/%E6%95%B0%E5%80%A4%E8%A7%A3%E6%9E%90,数値解析 - Wikipedia}
 * NUMERICAL RECIPES in C[日本語版]
 * @<href>{https://ja.wikibooks.org/wiki/高等学校数学II,WikiBooks 高等学校数学II}
