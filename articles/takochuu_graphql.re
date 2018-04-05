= GraphQLあるいはWeb API インターフェースについて

== 自己紹介
こんにちは！はじめまして。
株式会社エウレカでPairsのサーバーサイドを書いています。@takochuuです。@<br>{}

主にSPA / ネイティブアプリ向けのAPIを書く事が多いので、GraphQLをはじめとした
APIに用いられる技術と最近着目されているAPI向けの技術に関してこの章を書きたいと思います。


== はじめに
本章ではWeb APIに用いられる技術について解説します。

RESTの流行と共に、json形式でのRequest / Responseが主流となっていますが
GraphQL / Protocol Bufferのような新たな潮流が生まれています。
本章はこの2つの技術について焦点を当て、どのような箇所で活用が可能かについて示したいと思います。


== Web API インターフェースの歴史
APIとは言わずもがな、Application Programming Interface（アプリケーション・プログラミング・インタフェース）の略です。
Web界隈ではサービスの内外からCRUD操作を可能にする役割として浸透していますが、(TODO)元々はOSレベルの用語です。


Webの世界にAPIの概念が持ち込まれたのは、Ajaxの発見によるものが大きいと筆者は考えています。
Ajaxの発見により、WebServerがレンダリングされたHTMLを返すだけのものから、jsonを用いてデータを返却するものへと変化を遂げます。
上記の発見によりデータと表現としてのコードが分割されることになり、今日のSPAやネイティブアプリの発展につながっています。



(なんか図を入れる)



今でこそ使用されるプロトコル/データ形式はhttp/jsonが一般的になっていますが、それまではSOAP/XMLを利用していました。
SOAPでは、プラットフォームに依存しないプロトコルが採用されています。


SOAPで採用されているXMLには冗長性があり、Javascriptや動的型付け言語で扱うには面倒だという側面があります。
それに対してRESTで採用されることが多いjsonは、定義の完結さやJavascriptでそのまま扱えるということもあり、急速に普及することになりました。


== 課題
RESTはRoyFielding氏が2000年に発表した論文(@<href>{https://en.wikipedia.org/wiki/Representational_state_transfer})に基づいていますが
HTTP上の対話モデルをアーキテクチャとして定義しているに過ぎず、endpointのpathの定義の方法やバージョニング、エラー処理等については何の言及もありません。@<br>{}




実際のプロダクト開発として考えると、以下のような問題に直面したみなさんも多いのではないでしょうか。

1. 書く人によってendpointの命名方法が異なる@<br>{}
2. Request/Responseに一貫したSchema定義がないことにより、大規模開発時に一貫性が失われる(ところによりSchema定義を別途書くなどの面倒さを抱えることになる)@<br>{}
3. httpリクエストで対応仕切れない部分が出て来る(GETでの更新処理など)@<br>{}

上記のような状態に悩まされている読者の方もいらっしゃるのではないでしょうか。
実際、筆者が今まで開発してきたプロダクトもこのような状態で悩まされていました。
本章では、上記のような状態に悩まされている読者の皆様へ向けて技術を用いた解決方法を提示していきます。


== GraphQLとは何か
上記のような状態を解決するために筆者が注目している技術の一つとして、GraphQLがあります。
GraphQLとはFacebookが開発しているクエリ言語で、 2015年のReact.js Confで発表されました。


近年ではGithubのv4APIにて採用されていることや、2018年のDroidKaigiでgfxさんが発表していたこともあり
徐々に浸透しつつある技術なのではないでしょうか。

//image[GraphQL][]{
//}

GraphQLの大きな特徴として、Queryという構文でサーバーに対してQueryを発行することができます。
リソース更新のメソッドもあるものの現状は取得用途に使われる仕様だと考えておいたほうが良いでしょう。



それでは、サンプルとしてGithubのv4APIに対してGraphiQLで取得をしてみましょう。
GrapiQLとはブラウザ上でGraphQLのクエリを作成することができ、データ取得が可能なライブラリです。(TODO: ライブラリかどうかを表現考える)
https://developer.github.com/v4/explorer/


このような形でクエリを書いて実行するとサーバーからjson形式でレスポンスが返ってきている事がわかります。


//image[GraphiQL][デフォルトのクエリ]{
//}


クエリを書き換え、このようにしてみると返却されるレスポンスが変化していることがわかります。


//image[GrapiQL_mod][クエリを変更後]{
//}



これが、8-4で述べる利点と通ずる事になります。
余談ですがGrapiQLは、下記のようにドキュメントを閲覧する機能も実装しており、非常に強力なツールになっています。

//image[GrapiQL_Document][右側にドキュメントが表示されます]{
//}


== GraphQLを活用することの利点


前項目にてGraphiQLを使用し、クエリを変更するとレスポンスが変更されることがわかりました。
ではその特徴はプロダクト開発においてどのような利点があるのでしょうか？


まずは先程見た特徴の通りですが、不必要な情報を取得しないことによる計算リソースの節約です。
型があること。(TODO)


サーバーサイドではRequest/ResponseのSchemaが定義でき、クライアントとの認識統一が優位であることが挙げられます。
前述したSchema定義がない問題を解決することができます。


(技術で解決出来る問題をコミュニケーションで解決するのは面倒ですよね)


== GraphQLのgo実装(サーバーサイド)


ここからはいよいよサーバーサイドの実装に入ります。


今回、GraphQLのライブラリとして [github.com/graphql-go/graphql](https://github.com/graphql-go/graphql) を利用します。
現在頻繁にcommitされており、goでGraphQLを扱う際のライブラリではstar数が最大となっているのでこちらを利用します。


GraphQLを用いて読み込みを行う場合は、Queryという構文を使用します。
簡単にサーバーサイドの処理を実装してみます。


//emlist[][]{
  package main

  import (
  	"bytes"
  	"encoding/json"
  	"fmt"
  	"net/http"

  	"github.com/graphql-go/graphql"
  )

  var q graphql.ObjectConfig = graphql.ObjectConfig{
  	Name: "query",
  	Fields: graphql.Fields{
  		"id": &graphql.Field{
  			Type:    graphql.ID,
  			Resolve: resolveID,
  		},
  		"name": &graphql.Field{
  			Type:    graphql.String,
  			Resolve: resolveName,
  		},
  	},
  }

  var schemaConfig graphql.SchemaConfig = graphql.SchemaConfig{
  	Query: graphql.NewObject(q),
  }
  var schema, _ = graphql.NewSchema(schemaConfig)

  func executeQuery(query string, schema graphql.Schema) *graphql.Result {
  	r := graphql.Do(graphql.Params{
  		Schema:        schema,
  		RequestString: query,
  	})

  	if len(r.Errors) > 0 {
  		fmt.Printf("エラーがあるよ: %v", r.Errors)
  	}

  	j, _ := json.Marshal(r)
  	fmt.Printf("%s \n", j)

  	return r
  }

  func handler(w http.ResponseWriter, r *http.Request) {
  	bufBody := new(bytes.Buffer)
  	bufBody.ReadFrom(r.Body)
  	query := bufBody.String()

  	result := executeQuery(query, schema)
  	json.NewEncoder(w).Encode(result)
  }

  func main() {
  	http.HandleFunc("/", handler)
  	http.ListenAndServe(":8080", nil)
  }

  func resolveID(p graphql.ResolveParams) (interface{}, error) {
  	return 1, nil
  }

  func resolveName(p graphql.ResolveParams) (interface{}, error) {
  	return "hoge", nil
  }
//}

goにおけるGraphQLのサーバーサイドの実装では、このように schemaを定義して `graphql.Do` にschemaとクエリを渡す事で処理します。
`go run main.go` でサーバーを立てて、curlでデータを取得してみます。

//cmd{
  #!/bin/bash
  curl -X POST -d '{ id }' http://localhost:8080/
  curl -X POST -d '{ name }' http://localhost:8080/
  curl -X POST -d '{ id, name }' http://localhost:8080/
//}

サーバーから返却されたレスポンスはこちらです

//cmd{
  {"data":{"id":"1"}}
  {"data":{"name":"hoge"}}
  {"data":{"id":"1","name":"hoge"}}
//}

上記のように、curlで指定したfieldと対応する値が返却されていることがわかります。

//emlist[][]{
  "id": &graphql.Field{
  			Type:    graphql.ID,
  			Resolve: resolveID,
  		},
  },
//}

指定されたfieldに対するvalueはResolveに対して定義している `resolveID` と `resolveName` で値を生成して返却しています。
Queryに引数を渡すこともできます。
先程記述した、サーバーサイドのコードを一部書き換えて下記のようにします。

//emlist[][]{
  "id": &graphql.Field{
		"id": &graphql.Field{
			Type: graphql.ID,
			Args: graphql.FieldConfigArgument{
				"id": &graphql.ArgumentConfig{
					Type: graphql.Int,
				},
			},
			Resolve: resolveID,
		},
  }

  func resolveID(p graphql.ResolveParams) (interface{}, error) {
  	return p.Args["id"], nil
  }
//}

この状態で、先程実行したcurlを変更し、このようなクエリを投げます。

//cmd{
  curl -X POST -d '{ id(id: 100), name }' http://localhost:8080/
//}

レスポンスを見ると、このように引数として渡した `100` が idのvalueとして帰ってきていることがわかります。

//cmd{
    {"data":{"id":"100","name":"hoge"}}
//}
今回は端折っていますが、具体的にサービスを実装する際は `Resolve`にマッピングした関数に渡ってきた引数を利用して、データを引っ張ることになります。

=== Protocol Bufferとは何か

=== Protocol Bufferを活用することの利点

=== Protocol Buffer実装方法

== おわりに