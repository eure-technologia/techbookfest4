= swift-nioで始めるEvent Driven Network Application
はじめましてこんにちは。株式会社エウレカでiOSエンジニアとして主にUI/UX周りの開発を担当しています、@satoshin21です。
最近一眼カメラを入手し、夜な夜な風景を撮りっています。
大きなカンファレンスのカメラマンとしてデビューする事を夢見てカメラの勉強中です。

== try! Swift 2018とswift-nio
try! Swift 2018にて、最も盛り上がったセッションの一つとして、AppleのServer Infrastructure Teamの一人であるNorman Maurerによる「Event driven networking for Swift」があげられるかと思います。
このセッションの最後で、SwiftNIOがその日のうちに公開され、大きな歓声が巻き上がりました。
果たして、このSwiftNIOとは一体どのようなものなのでしょう？

== Event Driven Network Applicationとは
すっごい雑に書くよ！

Event Driven Network Applicationについて、同期・非同期、及びブロッキング・ノンブロッキングの概念から説明します。
ファイルやネットワーク通信におけるI/Oバウンドな処理について、。。。
まずは、同期的なHTTPリクエスト処理は以下の通りにかけるかと思います。

```swift
import Foundation

public func httpGet(url: URL) throws -> String {

    var responseData: Data?
    let semaphore = DispatchSemaphore(value: 0)
    let request = URLRequest(url: url)
    let task = URLSession.shared.dataTask(with: request)
    { (data, _, error) in

        responseData = data
        semaphore.signal()
    }
    task.resume()
    semaphore.wait()

    if let string = responseData.flatMap({String(data: $0, encoding: .utf8)}) {
        return string
    } else {
        throw ResponseError.somethingError
    }
}
```
ネットワークIOを開始後、そのまま通信が完了するまで待ちます。
基本的にswift-corelibs-foundationは同期的なURLRequestを行うAPIが存在しない為、DispatchSemaphoreにて無理やり同期敵に処理を行っています。
同期処理ですと、処理が完了しない限り次の処理が実行できません。これがいわゆるブロッキングです。

== 非同期処理
