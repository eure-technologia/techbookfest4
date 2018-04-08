= iOSとタイポグラフィ

== はじめに
はじめましてこんにちは。株式会社エウレカでiOSエンジニアとして主にUI/UX周りの開発を担当しています、@<href>{https://twitter.com/satoshin21, @satoshin21}です。
最近一眼カメラを入手し、とにかくいろんなものを撮っています。大きなカンファレンスのカメラマンとしてデビューすることを夢見てカメラの勉強中です。

フロントエンド開発において、まったく文字を取り扱わないで1つのアプリケーションやWebページを開発することはほとんどなく、フロント開発と文字は切っても切り離せない関係だと思います。それはもちろん、iOSアプリケーション開発においても同じで、UILabelやUITextViewなどを用いて日々開発を行っていると思います。
しかし、ただ単純に画像を表示する場合に比べ、（画像のメモリ管理は別として）@<b>{文字をアプリケーションで扱うときはさまざまな問題が発生します。}フォントによってレイアウトが崩れたり、デザイナーの要求に素直に（そしてカッコよく）答えられなかったりなど、なかなか最初の頃は一筋縄ではいきません。

この章ではタイポグラフィについて、SwiftとiOS開発の観点からざっくりと理解する為の手助けを目的としています。iOSエンジニアにとってはもちろん、タイbポグラフィについての基礎の部分を理解したい人のお役に立てればと思います。

== 書体の歴史と活版
タイポグラフィはさまざまな定義がありますが、一般的な概念としては@<b>{「文字をページ上に美しく配置する為の技法や工程」}のことを指します。iOS開発もUIFontを用い、UITextViewやUILabelなどのView上に配置する為の技法をタイポグラフィとして定義できるかと思います。

タイポグラフィは日本では古くは活版印刷で活用され、金属や木などに文字を彫り込み、それを判子状にしたものを1つの活版（組版）としてセットで使ってきました。活字を印刷する際には印刷したい文字を活版から選択してひとまとめにし、インクを塗って印刷します。英語のように記号と合わせて26文字強しかなかったものに比べ、日本語や中国語は膨大な文字が存在する為、それらを管理するのは非常に苦労したことが伺えます。
//image[kumihan][活版印刷で用いられた日本語の組版][scale=0.5]{
//}

DTP（DeskTop Publishing）の発達により、人間は膨大な数の文字の組み合わせである活版をフォントという形でデータとして扱い、パソコン上でとても簡単に文字を印刷物上に表現することができました。Web開発やネイティブアプリケーションにおいても同様で、我々はフォントを1つのセットとして取り扱うことで、膨大な数の文字の形を使い分けてアプリケーションのデザインを行うことができるようになりました。

フォントには文字コードの番号とそれに紐づく文字の形を組み合わせにして保持しています。iOSにおいてUIFontがフォントを表す為のクラスがであり、UIFont上で書体、及び文字の大きさやスタイルその他を指定して1つのフォントオブジェクトを生成し、文字の形を定義します。

==[column] 活版とフォントとグリフの関係性
活版とフォントとは、それぞれ文字の形をどのように定義するかを表す方法であり、書体はどちらかというと文字のまとまりを表す概念上の言葉です。なので、@<b>{基本的には書体はフォントであり、活版である}といえます。グリフはフォントそれぞれがもつ文字の形のことをグリフといいます。

== UIFont

UIFontにはフォントに関係するさまざまな情報が含まれます。主にUIFontにはフォントの形を表す情報と、フォントの位置、高さを示す情報の2種類が含まれています。

=== フォントの形を表す情報

フォントの形を表す情報としてUIFontでは主に、フォント名、フォントファミリー名、ポイントサイズを取得することができます。フォントは基本的にはひとつのフォントファミリーに属しています。フォントファミリーは基本的な書体はひとつとして、それらに太字やイタリック書体など、さまざまなスタイルを定義したものをフォントファミリーに所属するフォントとしてそれぞれ保持しています。上記のスタイル以外にも、たとえばHelvetica NeueがもつHelveticaNeue-CondensedBold、HelveticaNeue-Lightなどのさまざまなスタイルをもつフォントが存在します。

iOSでも、UIFontからフォント名などの情報を取得することが可能です。

//emlist[][]{
let font = UIFont(name: "Avenir-Medium", size: 12)!
font.fontName   // Avenir-Medium
font.familyName // Avenir
font.pointSize  // 12.0
//}

===[column] セリフ体
書体の大きな特徴としてセリフ体、サンセリフ体の2種類があります。
セリフとは主に欧文フォントで確認できる文字の線の橋につけられる線・飾りのことで、活版印刷が生まれた時はセリフをわざとつけることで文字を刻みつけやすくする為に用いられました。また、サンセリフのサンとは仏語でSans=「〜のない」という意味となり、我々が普段ゴシック体と呼んでいるものとほぼおなじ意味です。

iOSがシステムで保持するフォントはほぼすべてサンセリフ体のものが基本です。これは、一般的にデジタル上で表現されるフォントは、画面の解像度によってアンチエイリアスにより潰れてしまうことが懸念される為とされています。

//image[serif][セリフ体]{
//}

=== フォントの位置、高さの情報
フォントは同じポイントサイズでも、大きさや表示位置がさまざまです。フォントの高さは各フォントに依存し、また欧文フォントではある一定のラインに即してグリフが規則正しく表示されています。
iOSで取得できる主な高さ、位置情報は@<img>{font_heights}のようになっています。

//image[font_heights][iOSで取得できる高さ情報]{
//}

それでは、それぞれのプロパティについて見ていきましょう。

==== baseline
グリフ位置の基準となる線です。欧文フォントは基本的にこのbaselineを基本として配置されています。
iOSのAutoLayoutでは、仮に横並びになったUILabelなどをこのbaselineを基準に横並びに配置するためのアンカーポイントが用意されています

//emlist[][]{
// 1行目のbaselineに合わせる
minLabel.firstBaselineAnchor.constraint(
    equalTo: largeLabel.firstBaselineAnchor).isActive = true

// 最終行のbaselineに合わせる
minLabel.lastBaselineAnchor.constraint(
    equalTo: largeLabel.lastBaselineAnchor).isActive = true
//}

==== x height
baselineを基準に小文字xの高さを表します。

==== cap height
大文字Xの高さを表します。
基本的に大文字の大きさはcap heightと同じになりますが、フォントによってははみ出てしまう場合があります。

==== ascender
フォントによってはcap heightを上回る高さをもつグリフが存在する為、それらを含めた高さを表す為の数値です。

==== descender
小文字のyやgなど、baselineの下部にグリフが配置される場合があります。それらの下回る部分の高さのことをdescenderといいます。
iOSから取得できるdescenderはマイナス値となっている為、注意が必要です。

=== UIFontがもつ情報
上記の情報はUIFontに含まれていますが、iOSのUIFontには、それらに合わせて以下2つの情報が含まれています。

==== Line Gap（leading）
フォントのもつ行間の高さを表す値です。UIFontのもつleadingプロパティから取得できますが、実際にUIFontに指定されているフォントは限られており、
基本的には値が"0"となっています。leading値が確認できたフォントはHiraKakuProN-W3などがあります。

==== line height
ascenderとdescenderをあわせた高さ、実際のフォントの高さを表す・・はずですが、フォントによっては、line heightがフォント全てを表さない場合もあります。

こちらが、HiraKakuProNとSan Francisco（SF Pro Display）のbaselineとの位置関係を比べた図です。こちらの図ではleadingを表示していない為、HiraKakuProNでは、小文字のjがdescenderを上回ってしまっていることが分かるかと思います。

//image[font_leading_compare][2つのフォントと位置関係（Line Gap無し）]{
//}

そのためHiraKakuProN-W3などは、line heightを超えてしまうLine Gapを含めないと実際のフォントの高さとなりません。その為、iOSにおいてフォントのもつ正確な1行分の高さを取得する場合は、Line Gapを含めて計算する必要があります。

//emlist[][]{
let font = UIFont(name: "HiraKakuProN-W3", size: 12)!

font.xHeight // 6.54
font.capHeight // 9.192
font.ascender // 10.56
font.descender // -1.44
font.leading // 6.0
font.lineHeight // 12.0

let trueLineHeight =
    abs(font.descender)
        + font.ascender
        + font.leading // 18.0
//}

== 日本語におけるフォント
日本語フォントは欧文フォントと違い、baselineやascender, descenderなどの高さを考慮していません。これは、冒頭で説明していた活版印刷の頃における欧文と日文のレイアウト方法に依存します。
@<b>{日本語は四角形の中に収まる形でグリフを表し、欧文フォントのようにbaseline上に一定のラインを基準としたグリフ配置はしていません。}その為、UIFontで取得できるascenderなどの値は日本語には適用されませんが、欧文フォントに合わせる形でグリフの位置が定義されている為、leadingを考慮した高さを取得すれば、基本的にはみ出てしまうことはありません。

== UITextView
iOSでテキストを表示する場合は、UILabelやUITextViewを用いるかと思います。特にUITextViewはさまざまなカスタム可能なスタイル情報を使用してテキストの表示、編集、表示形式の変更が可能です。皆さんも普段のiOS開発において使用する機会が多いViewの1つだと思います。

UITextView上にテキストを表示する為に、内部では主にNSTextStorage, NSTextContainer, NSLayoutManagerの3つのクラスを用いてテキストの管理を行っています。

=== NSTextStorage
@<b>{NStextStorageは主に何（テキスト）を表示するかを管理}しています。NSMutableAttributedStringのサブクラスで、NSTextStorageのもつ内容や属性に変更があった場合、それをNSLayoutManagerに通知してテキストの再レイアウトを行うことができます。

UITextViewの @<code>{ textView.text = "foo" }などで文字列などを指定する時、TextViewは自身がもつNSTextStorageを変更します。NSTextStorageから直接attributesなどを変更することも可能です。

//emlist[][]{
textView.textStorage.beginEditing()
textView.textStorage.addAttributes(
    [.backgroundColor: UIColor.gray],
    range: NSRange(location: 0, length: 10))
textView.textStorage.endEditing()
//}

また、NSTextStorageは複数のNSLayoutManagerを通知先として保持することができます。
1つのUITextViewの編集内容を別のUITextViewに反映したい場合、NSLayoutManagerを渡すことで別のUITextViewへその変更を反映させることもできます。ただし、NSLayoutManager経由では通知先のNSTextStorageには変更が走らない為、あくまで変更内容がレイアウト上に反映されるだけなので注意が必要です。

//emlist[addLayoutManager][swift]{
textView2.text = "foo"
textView.textStorage.addLayoutManager(textView2.layoutManager)
textView.text = "bar"

textView2.text // textView2の表示は"bar"となっているが、"foo"が返される
//}

=== NSTextContainer
@<b>{NSTextContainerはNSTextStorageがもつ内容をどのように表示するかを管理}しています。
UITextView上に表示するテキストの座標と形状情報を持っています。UITextView上のスクロール可能な領域に関するレイアウト情報を持っているようなイメージで、座標の主な情報は@<img>{textview_margins}ような関係性となっています。

//image[textview_margins][UItextViewの各種マージン・インセット]{
//}

NSTextContainerや、UITextViewのもつ座標情報は次のように変更することができます。

//emlist[][]{
let paragraphStyle = NSMutableParagraphStyle()
paragraphStyle.firstLineHeadIndent = 50
paragraphStyle.headIndent = 10

textView.attributedText = NSAttributedString(string: "あのイーハトーヴォのすきとおった風、夏でも底に冷たさをもつ青いそら、うつくしい森で飾られたモリーオ市、郊外のぎらぎらひかる草の波。", attributes: [.paragraphStyle: paragraphStyle])

textView.textContainer.lineFragmentPadding = 20
textView.textContainerInset = UIEdgeInsets(
    top: 10,
    left: 20,
    bottom: 10,
    right: 10)
//}

UITextView, NSTextStorage（AttributedString）, NSTextContainerそれぞれにグリフの座標、及びTextContainerの表示領域に関する座標指定方法がある為、UITextView内を扱う場合は、表示領域とそれぞれの担当領域を判断し、適切な設定を行いましょう。

またNSTextContainerはNSAttributedStringでカバーできない、テキスト全体の描画方法についても指定することができます。
たとえば、NSTextContainerにはexclusionPathsを持っており、こちらを指定することでNSTextContainerで表示しない領域をUIBezierPathを用いて指定することが可能です。

//emlist[][]{
let triangle = UIBezierPath()
triangle.move(to: CGPoint(x: 250, y: 30))
triangle.addLine(to: CGPoint(x: 300, y: 150))
triangle.addLine(to: CGPoint(x: 200, y: 150))
triangle.close()

textView.textContainer.exclusionPaths = [triangle]
//}

こちらの出力結果が以下となります。
exclusionPathsで指定した所を抜けてテキストがレイアウトされていることが分かると思います。

//image[text_container][exclusionPath指定時の出力結果]{
//}

==== NSLayoutManager
NSLayoutManagerは上記2つのクラスを保持し、NSTextStorageのもつ内容をNSTextContainerのもつ座標・レイアウト情報をもとにUITextView上に表示するためのコントロールを行います。
UITextViewはUIFontから生成されるグリフやスタイル情報をキャッシュし、レイアウト処理に関わるボトルネックを最小限にしています。

NSLayoutManager, NSTextContainer, NSTextStorageの関係性はこのようになっています。

//image[textview][TextViewに関わるクラスの関係性][scale=0.5]{
//}

UITextViewにはこのように、さまざまなクラスと連携し、画面上にテキスト表示を可能にしています。
柔軟なタイポグラフィをiOS上で実現するためにUITextViewはさまざまなAPIを提供しているため、各クラスの特性を掴み快適なタイポグラフィを実現しましょう。

== おわりに
今回はUIFontやUITextViewを中心に、iOSのタイポグラフィに関わる部分についてざっくりとまとめてみました。テキストレイアウトなどはデザイナー領域のように感じられ、それほど関心も高くないかもしれませんが、
iOS SDKのCoreTextをベースとした各APIは高度且つ柔軟なデザイン要求にも簡単に応えられるような高機能なものがたくさん存在しています。
タイポグラフィを理解して、デザイナーと一緒に楽しく、そして最高のアプリを作っていきましょう！
