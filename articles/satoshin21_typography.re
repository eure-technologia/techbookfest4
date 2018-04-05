= iOSとタイポグラフィと私

== はじめに
はじめましてこんにちは。株式会社エウレカでiOSエンジニアとして主にUI/UX周りの開発を担当しています、@satoshin21です。
最近一眼カメラを入手し、とにかくいろんなものを撮っています。大きなカンファレンスのカメラマンとしてデビューする事を夢見てカメラの勉強中です。

== iOSとタイポグラフィ
フロントエンド開発において、で全く文字を取り扱わないで一つのアプリケーションやWebページを開発することは全くといいほど、フロント開発と文字は切っても切り離せない関係だと思います。それはもちろん、iOSアプリケーション開発においても同じで、`UILabel`や`UITextView`などを用いて日々開発を行っていると思います。
しかし、ただ単純に画像を表示する場合に比べ、(画像のメモリ管理は別として)文字をアプリケーションで扱うときは様々な問題が発生します。フォントによってレイアウトが崩れたり、文字をTextViewやUILabelで表示した際にその高さ計算のパフォーマンスに悩まされ、またStringから特定の文字数分だけ抜き出す場合に意図せぬ結果を得たりなど、なかなか最初の頃は一筋縄ではいきません。

文字コードは文字をいかにバイナリで表現するかを表す為の規約であり、タイポグラフィは文字を様々なフォントを用いつつ、美しくレイアウト上に表現するための手法を指します。文字コードはどちらかというとエンジニアよりの内容であり、タイポグラフィはデザイナー寄りの話になるかと思います。フロントエンドエンジニアはデザイナーやバックエンドエンジニアと連携しつつ最適なアプリケーション開発を進めていかなければならないので、そのどちらも理解しなければなりません。iOSエンジニアにとってもそれは同じで、どちらの概念について学ぶ必要があるかと思います。

この章ではタイポグラフィと文字コードについて、SwiftとiOS開発の観点からざっくりと理解する為の手助けを目的としています。iOSエンジニアにとってはもちろん、文字コードやタイポグラフィについての基礎の部分を理解したい人のお役に立てればと思います。

== 書体の歴史と活版
タイポグラフィは様々な定義がありますが、一般的な概念としては「文字をページ上に美しく配置する為の技法や工程」の事を指します。iOS開発もUIFontを用い、UITextViewやUILabelなどのView上に配置する為の技法の事をタイポグラフィとして定義できるかと思います。

タイポグラフィは日本では古くは活版印刷で活用され、金属や木などに文字を彫り込み、それを判子状にしたものを一つの活版(組版)として一つのセットとして使ってきました。活字を印刷する際には印刷したい文字を活版から選択してひとまとめにし、インクを塗って印刷します。英語のような26文字強しかなかったものに比べ、日本語や中国語は膨大な文字が存在する為、それらを管理するのは非常に苦労したことが伺えます。
//image[kumihan][組版][scale=0.5]{
    組版
//}

DTP(DeskTop Publishing)の発達により、人間は膨大な数の文字の組み合わせ、活版をフォントという形でデータとして扱うことでパソコン上でとても簡単に文字を印刷物上に表現することができました。Web開発やネイティブアプリケーションにおいても同様で、我々はフォントを一つのセットとして取り扱うことで、膨大な数の文字の形を使い分けてアプリケーションのデザインを行うことができるようになりました。

フォントには文字コードの番号とそれに紐づく文字の形を組み合わせにして保持しています。iOSにおいてこのフォントを表す為のクラスが存在し、UIFont上で書体、及び文字の大きさやスタイルその他を指定して一つのフォントオブジェクトを生成し、文字の形を定義します。

//quote{
活版とフォントとは、それぞれ文字の形をどのように定義するかを表す方法であり、書体はどちらかというと文字のまとまりを表す概念上の言葉です。なので、基本的には書体≒フォントであり、書体≒活版であるといえます。グリフはフォントそれぞれが持つ文字の形の事をグリフといいます。
//}

== UIFont

UIFontにはフォントに関係する様々な情報が含まれます。主にUIFontには2種類の情報が含まれています。

フォントの形を表す情報
フォント名、フォントサイズ
フォントの高さの情報
baseline, lineHeight

=== フォントの形を表す
//emlist[][]{
let font = UIFont(name: "Avenir-Medium", size: 12)!
font.fontName   // Avenir-Medium
font.familyName // Avenir
font.pointSize  // 12.0
//}

フォントの形を表す情報としてUIFontでは主に、フォント名、フォントファミリー名、ポイントサイズを取得することができます。フォントは基本的には一つのフォントファミリーに属しています。フォントファミリーは基本的な書体は一つとして、それらに太字やイタリック書体など、様々なスタイルを定義したものをフォントファミリーに所属するフォントとしてそれぞれ保持しています。上記のスタイル以外にも、例えばHelvetica Neueが持つ`HelveticaNeue-CondensedBold`、`HelveticaNeue-Light`などの様々なスタイルを持つフォントが存在します。

//quote{
書体の大きな特徴としてセリフ体、サンセリフ体の2種類があります。
セリフとは主に欧文フォントで確認できる文字の線の橋につけられる線・飾りのことで、活版印刷が生まれた時はセリフをわざとつけることで文字を刻みつけやすくする為に用いられました。
iOSがシステムで保持するフォントはほぼすべてサンセリフ体のものが基本です。これは、一般的にデジタル上で表現されるフォントは、画面の解像度によってアンチエイリアスにより潰れてしまうことが懸念される為とされています。
//}

//image[serif][serif]{
  serif
//}

=== フォントの高さの情報
フォントの高さを表す情報としては、特に欧文フォントにおいて様々なものが存在します。フォントの持つ高さ情報は以下のようなものをそれぞれ表しています。

//image[font_heights][font]{
fontの高さ
//}

==== baseline
欧文フォントの書体をグリフで表現するときの基準となる線

==== x height
小文字のxの高さ

==== capsHeight
大文字Xの高さ
基本的に大文字の大きさはcapHeightと同じになるが、はみでる場合もある

==== ascender
フォントによって、caps heightを上回る高さを持つグリフが存在する為、それらを含めた高さ
iOSでは基本的にbaselineからcaps heightとはみでる部分の高さを表現しているが、場合によってはみでる部分のみのことをascenderと呼ぶ場合も存在

==== descender
baselineを基準に、小文字のgなどbaselineの下部分をカバーする高さのこと

==== line height
ascenderとdescenderをあわせたラインの高さ

==== leading
フォントの持つ行間の高さ
持っていないフォントが多数

== Swiftで取得する場合
//emlist[][]{
font.xHeight
font.capHeight
font.ascender
font.descender
font.leading
font.lineHeight
let lineHeight = abs(font.descender) + font.ascender
//}

== 日本語におけるフォント
日本語フォントと欧文フォントと違い、baselineやascender,descenderなどの高さを考慮していません。これは、冒頭で説明していた活版印刷の頃における欧文と日本語のレイアウト方法のち外に依存します。
日本語は四角形の中に収まる形でグリフを表し、以下の通り欧文フォントのようにbaseline上に一定のラインを基準としたグリフ表現はしていません。その為、UIFontで取得できるascenderなどの値は日本語には適用されませんが、欧文フォントに合わせる形でグリフの位置が定義されている為、lineHeightをはみ出るような事はありません。

== UITextViewにおけるタイポグラフィ
iOSでテキストを表示する場合は、`UILabel`や`UITextView`を用いるかと思います。特にUITextViewは様々なカスタム可能なスタイル情報を使用してテキストの表示、編集、表示形式の変更が可能です。皆さんも普段のiOS開発において使用する機会が多いViewの一つだと思います。

UITextView上にテキストを表示する為に、内部では主に3つのクラスを用いてテキストの管理を行っています。

== NSTextStorage
NStextStorageは主に何(テキスト)を表示するかを管理しています。NSMutableAttributedStringのサブクラスで、TextStorageの持つ内容や属性に変更があった場合、それをNSLayoutManagerに通知してテキストの再レイアウトを行うことができます。

UITextViewの.textなどで文字列などを指定する時、TextViewの持つTextStorageを変更します。TextStorageから直接attributesなどを変更する事も可能です。

//emlist[][]{
textView.textStorage.beginEditing()
textView.textStorage.addAttributes(
    [.backgroundColor: UIColor.gray],
    range: NSRange(location: 0, length: 10))
textView.textStorage.endEditing()
//}

また、TextStorageは複数のLayoutManagerを持つことができます。
その為、1つのTextViewの編集内容を別のTextViewに反映したい場合、LayoutManagerを渡すことで別のTextViewへその変更を反映させる事もできます。ただし、LayoutManager経由で通知先のTextStorageには変更が走らず、あくまで変更内容がレイアウト上に反映されるだけなので注意が必要です。

//emlist[][]{
textView2.text = "foo"
textView.textStorage.addLayoutManager(textView2.layoutManager)
textView.text = "bar"
textView2.text // foo
//}

== NSTextContainer
NStextContainerはTextStorageが持つ内容をどのように表示するかを管理しています。
UITextView上に表示するテキストの座標と形状情報を持っています。TextView上のスクロール可能な領域に関するレイアウト情報を持っているようなイメージであり、座標の主な情報は以下のような関係性となっています。
TextContainerや、TextViewの持つ座標情報は以下のように変更することができます。

//emlist[][]{
let paragraphStyle = NSMutableParagraphStyle()
paragraphStyle.firstLineHeadIndent = 50
paragraphStyle.headIndent = 10

textView.attributedText = NSAttributedString(string: "あのイーハトーヴォのすきとおった風、夏でも底に冷たさをもつ青いそら、うつくしい森で飾られたモリーオ市、郊外のぎらぎらひかる草の波。", attributes: [.paragraphStyle: paragraphStyle])
//textView.text = "あのイーハトーヴォのすきとおった風、夏でも底に冷たさをもつ青いそら、うつくしい森で飾られたモリーオ市、郊外のぎらぎらひかる草の波。"
textView.textContainer
textView.textContainer.lineFragmentPadding = 20
textView.textContainerInset
textView.textContainerInset = UIEdgeInsets(top: 10, left: 20, bottom: 10, right: 10)
//}

また、TextContainerはNSAttributedStringでカバーできない、テキスト全体の描画方法についても指定することができます。
例えば、textContainerにはexclusivePathを持っており、こちらを指定することでTextContainerで表示しない領域をUIBezierPathを用いて指定することが可能です。

//emlist[][]{
let triangle = UIBezierPath();
triangle.move(to: CGPoint(x: 250, y: 30));
triangle.addLine(to: CGPoint(x: 300, y: 150));
triangle.addLine(to: CGPoint(x: 200, y: 150));
triangle.close()
textView.textContainer.exclusionPaths = [triangle]
//}

//image[text_container][TextContainerの出力]{
Text Containerの出力
//}

=== NSLayoutManager
NSLayoutManagerは上記2つのクラスを保持し、TextStorageの持つ内容をTextContainerの持つ座標・レイアウト情報をもとにTextView上に表示するためのコントロールを行います。
