= Go言語数値計算ことはじめ、あるいはなぜGo言語で数値計算が流行らないのか

== はじめに
はじめましてこんにちは。ここ2年くらいはもっぱらGo言語でサーバサイドAPIを作り続けている@yu81です。時々機械学習系のこともやったりしています。
今回は、Go言語における数値計算を取り巻く状況について、少しではありますが現状の調査と実際の実装を行い考察してみたので、これについて書きたいと思います。

== 数値計算とは
数値計算といって、イメージが湧く人と湧かない人がいると思います。情報科学系の人よりも、他の理工系の自然科学系の大学に行っていた人の方が身近な概念かもしれません。
辞書的な定義をWikipedia先生に譲り、その引用を次に記します。（一部体裁の改変あり）

@<i>{引用ここから}

//lead{
  数値解析（すうちかいせき、Numerical Analysis）は、数学および物理学の一分野で、代数的な方法で解を得ることが不可能な解析学上の問題を（通常は有限精度の）数値を用いて近似的に解く手法に関する学問である。

  史上最初の数学的記述のひとつとして、バビロニアの粘土板 YBC 7289 を挙げることができる。これは六十進法で √2 （単位矩形の対角線の長さ）を数値的に近似したものである。三角形の辺の長さを計算できること（そして、平方根を計算できること）は、特に建築において重要である。たとえば、縦横それぞれ2メートルの正方形の壁の対角線は √8 ≈ 2.83 メートルとなる。

  数値解析は、このような実用的計算の長い伝統に続くものである。バビロニアの √2 の近似のように、現代の数値解析も厳密な解を求めようとするものではない。なぜなら、厳密な解を有限時間で求めることは不可能だからである。その代わりに、数値解析の多くは、ある程度の誤差の範囲内の近似解を求めようとする。

  数値解析は自然科学および工学のあらゆる分野に応用がある。計算言語学や社会統計学のように、人文科学や社会科学でも重要である。

  コンピュータ以前は、数値的な手法は数表と、大きな計算になると機械式計算機による補助も近代には使われたものの、紙とペンによる膨大な途中経過をダブルチェックして行われる大変な作業であった。（個々の計算はそろばんにより行われるかもしれないが、そろばんで「膨大な途中経過」は扱えない。計算尺も用途によっては有用だが、有効数字が限られており、それでは間に合わない分野も多い）

  コンピュータの発達以降は、従来から見るとその桁外れな計算力により応用分野が大きく広がったとともに、コンピュータ以前からのノウハウに加え、コンピュータによる計算の特性を正しく理解して使うことも重要になった。

  常微分方程式は天体（惑星、恒星、小宇宙）の軌道の計算に登場する。資産管理には最適化が利用されている。数値線型代数はデータ解析に不可欠である。確率微分方程式とマルコフ連鎖は、医学や生物学における生体細胞のシミュレーションの基本である。
//}

@<i>{引用ここまで}

代数的な方法で解を得ることが不可能というのは、誤解を恐れず身近な感覚で例を挙げると、紙とペンを使って時間を掛けさえすれば解けるような方程式ではない、というところでしょうか。
もちろん数値計算は代数的に解ける問題に対しても適用可能ですが、独自の価値があるのはそうでない問題に対して、ということですね。
そして、学校教育内で受けるテストに出てくる問題は、紙とペンとちょっとの時間があれば解けるように設定されていますが、現実にあるほとんどの方程式はこれが不可能なので、数値計算が本質的な手法となる、ということでしょう。

自然科学、たとえば物理学において、ある現象を理解するためにはさまざまな要素を微分的に考え、微分方程式を立ててそれを解くことでその現象に対する知見を得るという流れがあります。
次に示す流体力学の基礎方程式であるナヴィエ・ストークス方程式は、そのままでは解を求めることが不可能であるため、条件に応じた適切な近似系を導出して解いたり、数値計算で解いて利用します。

//texequation{
(\vec{v} ・ \nabla) \vec{v} = \mathrm{grad}(\frac{1}{2}\vec{v}^{2}) - \vec{v} \times \vec{\omega}
//}


== Goにおける数値計算の現状
さて、Go言語における数値計算ですが、事例があまりないといってよいでしょう。
ポインタの概念はあるものの、C言語のそれと異なり使い方を間違えるとメモリ破壊やセグメンテーションフォールトの原因となるようなものではなく、言語仕様もコンパクトで身につけやすく、処理によっては普通に書いてもC++に比肩する速度を出せるため、プログラミングを本業としない科学者の利用には非常に適していると思うのですが。


=== 言語仕様・標準ライブラリ
Goの言語仕様や標準ライブラリについて、数値計算にとって直接的に有用なものはさほどありません。ですが、そのシンプルな仕様ゆえに、計算という本分に集中できるといえるでしょう。
組み込み型に複素数型がありますが、自然科学で直ちに利用可能なものはこれくらいでしょうか。

=== 既存ライブラリ
==== Gonum

@<href>{http://www.gonum.org/,Gonum（http://www.gonum.org/）} というものがあります。内部的にはBLAS（Basic Linear Algebra Subprograms）とLAPACK（Linear Algebra PACKage）という、わりと昔から存在するベクトルや行列、線形代数でよく用いる演算の共通ルーチン集のようなライブラリの、インターフェースを維持したGo版実装となっています。
元の実装がC言語屋FORTRANであるため、歴史を感じる作りになっています。

=== 他言語との比較
2018年現在において、数値計算に関してPythonに言及することは避けられないでしょう。
機械学習でよく用いられるPythonですが、さまざまな計算においてNunPyやSciPyが用いられています。非常に高度なことばかり行っていて常人には理解できない計算ばかりという先入観を持ってしまうのですが、たとえばニューラルネットワークの計算も大雑把にいってしまえば座標変換と活性化関数の作用を大量に繰り返しているだけなので、高等数学といっても大学1,2年生なら理解できるレベルではあるはずです。
機械学習の盛り上がりもあり、現在数値計算でもっとも用いられている言語といえばPythonといっても過言ではなく、NumPyがもっとも用いられている数値計算ライブラリというのもまた然り、というところでしょうか。
NumPyの内部実装のコア部分はC言語やFORTRANによって実装されており、NumPyのインターフェースはこれらの高級なラッパーとして振る舞うため、実際の動作は通常のPythonを用いた数値計算に比べると極めて高速に動作します。

== Goでスクラッチで書いてみる（とりあえず高校数学レベル）

=== 複素数
簡単に複素数の復習をしておきましょう。まず、普通の数は2乗すると必ず正の値になりますが、そうならない数を定義します。すなわち、

//texequation{
i^{2} = -1 \\
//}
//texequation{
\Rightarrow i = \sqrt(-1) （正の平方根を採用）
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
ベクトルに関しても簡単に復習をしておきましょう。
ベクトルはプログラマ的な文脈でいえば、いわゆる配列や C++の std::vector クラスのようなイメージになりますが、数学や数値計算的な文脈では少し違います。
数学的な定義としては、@<b>{向きと大きさを持った量}、となります。
また、全てが通常の数と同じではありませんが、加減算が成立する、実数倍することが可能（スカラー積）、加減算について結合法則と分配法則が成立する、などの性質を持ちます。数学的には群と大体同じ性質を持っています。
上記のようなかしこまった定義となりますが、数値計算のプログラミングで用いるような場合、これらの性質に対して神経質になることはさほどないと思います。

次に、ベクトルに関する代表的な演算について、サンプル実装を試みています。

#@# https://play.golang.org/p/G328POVxTeX
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

  func (v *Vector) Add(v2 *Vector) *Vector {
  	if !hasSameDimension(v, v2) {
  		return nil
  	}
  	dim := v.Dimension()
  	newVector := NewVector(make([]float64, dim))
  	for i := 0; i < dim; i++ {
  		newVector.elements[i] = v.elements[i] + v2.elements[i]
  	}
  	return newVector
  }

  func (v *Vector) Multiply(coefficient float64) *Vector {
  	if v == nil {
  		return nil
  	}
  	newElements := make([]float64, v.Dimension())
  	copy(newElements, v.Elements())
  	newVector := NewVector(newElements)
  	for i := 0; i < v.Dimension(); i++ {
  		newVector.elements[i] = coefficient * v.elements[i]
  	}
  	return newVector
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

  	fmt.Println(*v1.Add(v2))
  	fmt.Println(*v1.Multiply(3.0))

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
行列もプログラマ的な感覚でいうと、ベクトルが1次元配列であることに対して、2次元配列のようなものといえます。
行と列の数によっては2つの行列について加減算や乗算が定義可能です。
ベクトルは、行または列が1つである行列として考えることができます。

サンプルコードについては力尽きたのでまたいずれ……。

=== ニュートン法
ニュートン法とは、ある方程式 f(x) = 0 の解を求めるために反復的な計算を行うアルゴリズムの古典的なもののひとつです。
f(x) は反復計算を行う際の定義域において微分可能であればどのような関数でも利用可能です。
微分可能性を要求しているのは、背後にある理論によるものです。
ある程度ざっくりいうと、f(x)をある点についてテイラー展開を行い解の近似式を得て、それを収束する漸化式とみなすことが多くの場合に可能であるから、ということですが、詳細は省略します。

次に、ニュートン法のサンプル実装を示します。平方根と立方根を求める関数の実装ですが、これをニュートン法として再実装しており、さらにニュートン法の実装は関数とその1次微分を注入可能にしてみました。

#@# https://play.golang.org/p/u_YzyeXyZrA
//list[go-newton-def][Go言語におけるニュートン法の例]{
  package main

  import (
  	"fmt"
  	"math"
  )

  func CubicRoot(v float64) float64 {
  	e := func(x float64) float64 { return x*x*x - v } // f(x) = x^3 - v = 0
  	d := func(x float64) float64 { return 3 * x * x } // f'(x) = 3x^2
  	return Newton(e, d, v)
  }

  func SquareRoot(v float64) float64 {
  	e := func(x float64) float64 { return x*x - v } // f(x) = x^2 - v = 0
  	d := func(x float64) float64 { return 2 * x }   // f'(x) = 2x
  	return Newton(e, d, v)
  }

  func Newton(
  	equation func(x float64) float64,
  	derivative func(x float64) float64,
  	initial float64) float64 {
  	z := initial
  	const (
  		maxLoopCount        = 10000
  		convergenceCriteria = 0.00000001
  	)
  	for i := 0; i < maxLoopCount; i++ {
  		previous := z
  		z = z - equation(z)/derivative(z)
  		diff := math.Abs(z - previous)
  		if diff <= convergenceCriteria {
  			break
  		}
  	}
  	return z
  }

  func main() {
  	fmt.Println(SquareRoot(2))
  	fmt.Println(CubicRoot(2))
  }
//}


== 実際に書いてみてから、再び他言語との比較
いくつかの数学的な概念の復習と、そのGo言語実装について簡単に試みてきました。
筆者のバックグラウンドとして、学生時代にたどたどしいC言語で数値計算を行ったことがある、というのもあるのですが、C++よりも書きやすいベターC言語のようなコードとして実装できていると思います。
Go言語では組み込みのテスト機構の一部にマイクロベンチマークの機構があるのですが、確認したところ上記のコードはメモリアロケーションも少なく、さほど遅くない処理になっていることが確認できます。ただし、C言語、C++及びPythonとの比較が出来なかったので、次回の課題としたいところです。

ここで、NumPyと上記のコードとの比較をしてみたいと思います。
NumPyにおいては、numpy.ndarray (N-dimensional array)というデータ構造を用いてベクトル、行列、テンソルといった概念を統一的に扱えるようになっています。
次に、NumPyの公式ドキュメントを参照し、ベクトルに関してGoのサンプル実装のうち、加減算と内積、なす角を求める部分について同等の処理を実装してみます。

//list[python-vector-def][NumPyを用いた基本的なベクトル演算]{
  import numpy as np

  v1 = np.array([1.0, 2.0])
  v2 = np.array([4.0, 5.0])

  print(v1+v2)
  print(3.0*v1)
  print(v1*v2)

  def angle(v1, v2):
      cosine = np.dot(v1, v2) / (np.linalg.norm(v1) * np.linalg.norm(v2))
      return np.arccos(cosine)

  print(angle(v1, v2))
//}

Pythonはマジックメソッドを用いた演算子オーバーロードが可能であるため、numpy.ndarray はプリミティブ型と同様の記法で演算が可能になっています。
内積はdot関数により計算可能です。
また、ノルムなども同様に、線型代数の為のサブパッケージlinalgの関数を用いて計算可能です。

なるほど、数学、特にベクトルや行列を用いる線型代数のような処理を行う場合は、Go言語を用いて数値計算するよりも可読性も良く、数学的により直感的に書くことができ、手間がはるかに小さいような気がします。
今回は筆者のサンプルコードレベルのスクラッチ実装と比較しましたが、それ以上にgonumにおけるBLASやLAPACKと同等のインターフェースは非常に歴史的なものであるため、慣れ親しんでいない場合は非常に書きづらく可読性も悪くなることが予想されます。
もう少し深掘りが必要なことは重々承知ながらも、Goで数値計算を行うことは良く整備され使いやすく高速なライブラリがすでに普及している他言語で存在するため、あまり適さない可能性が高いと考えられます。
シンタックスレベルでC言語との類似性があるからといって、C言語で用いていたような某I社のコンパイラのような最適化を行うものではないです。
ですので、プログラミングを専門としない科学者の観点からも、C言語からの乗り換えを考えた場合に、C言語に少し似た別言語を学ぶよりも、より読み書きしやすく速度がC言語に比肩する処理速度をもつライブラリが使える別言語を用いた方がいいという判断になりそうです。

== まとめ
次のようにまとめます。

 * Go言語においては数値計算に用いられている状態ではない。
 * Go言語において、基本的な数学的なデータ構造の実装については問題なく行えるが、可読性はC言語より少しよい程度。
 ** PythonでNumPyを用いた方がより数学的に直感的で可読性もパフォーマンスも高い。
 ** パフォーマンスについては追加調査が必要
 * 科学者がC言語を使っているのは歴史的経緯と某社の最適化が強いコンパイラがある環境であることが考えられるので、C言語からGo言語に乗り換えるメリットは少ないように考えられる。
 ** Python/NumPy であればメリットが大きい可能性がある。

== 参考文献

 * @<href>{https://ja.wikipedia.org/wiki/%E6%95%B0%E5%80%A4%E8%A7%A3%E6%9E%90,数値解析 - Wikipedia}
 * @<href>{https://ja.wikipedia.org/wiki/%E3%83%99%E3%82%AF%E3%83%88%E3%83%AB%E7%A9%BA%E9%96%93,ベクトル空間 - Wikipedia}
 * @<href>{https://ja.wikipedia.org/wiki/%E3%83%8B%E3%83%A5%E3%83%BC%E3%83%88%E3%83%B3%E6%B3%95,ニュートン法 - Wikipedia}
 * NUMERICAL RECIPES in C[日本語版]
 * @<href>{https://ja.wikibooks.org/wiki/高等学校数学II,WikiBooks 高等学校数学II}
 * @<href>{https://www.amazon.co.jp/dp/4796112367,数学コメンタール -  山本　茂年\, 小川　徹夫\, 阿部　雄二\, 荒木　重蔵}
 * @<href>{https://www.amazon.co.jp/dp/4274219631,Pythonによる機械学習入門 - 株式会社システム計画研究所}
