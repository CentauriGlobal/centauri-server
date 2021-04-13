# centauri
# centauri 服务端 游戏币类API v3 Postman脚本使用指南

## 简介

[Postman](https://www.getpostman.com/products)是一款业界知名的API开发工具。为了方便centauri接入业务快速上手，我们利用Postman强大的脚本扩展能力，编写了一套游戏币类API v3的脚本。通过这套脚本，业务可以在Postman上轻松的构造并发送游戏币类API请求。

## 导入

脚本和相关配置，统一保存在[Collection V2](https://go.pstmn.io/collection-v2)的`JSON`文件中。开发者可以点击Postman界面左上角的`Import`按钮或者通过`File`中的`Import`发起导入。选择本地的`centauri_server.postman_collection.json`，点击确认后，导入便完成了。![image](https://user-images.githubusercontent.com/82426537/114562705-5387ab80-9ca1-11eb-92df-4ab46cc026e4.png)

你会发现在左侧Collection下新增了名为`centauri_server`的一组请求。

## 配置脚本

选中`centauri_server`，右键`Edit`进入Collection的配置页面。如图所示。

![image](https://user-images.githubusercontent.com/82426537/114562841-74e89780-9ca1-11eb-9dbe-0df49ab04c23.png)

在弹出的`Edit Collection`的浮层上部的多个分栏中，找到`Pre-request Scripts`一栏。

![image](https://user-images.githubusercontent.com/82426537/114562892-7fa32c80-9ca1-11eb-80b7-8acc3bbee594.png)

其中红色方框为使用说明，需要配置的参数和请求的参数。

设置全局的appkey，这里的appkey是接入centauri，申请应用的时候生成的，可在centauri的接入界面找到（妥善保存）。

点击后上角的`Environment`和下方的`Edit`，设置`Global`参数APP_KEY，如下所示：

![image](https://user-images.githubusercontent.com/82426537/114562941-8b8eee80-9ca1-11eb-97ce-4f128855b261.png)

## 配置请求

现在回到请求的配置界面吧。接下来，你可以按照协议去设置请求的信息了，如方法、URL、请求参数、Body。最后，按照使用说明配置两项`{{ts}}`、`{{sig}}`：
![image](https://user-images.githubusercontent.com/82426537/114562996-98abdd80-9ca1-11eb-87d9-e79301ec9da6.png)
![image](https://user-images.githubusercontent.com/82426537/114563015-9ea1be80-9ca1-11eb-80bb-12f6e11e6d82.png)

## 发起请求

点击地址栏右侧的`Send`按钮，发送你的请求吧。

1.点击左下方的`console`，可以看到计算`V3签名`的各步骤的中间关键结果，以及最终算出的签名`sig`；

2.最终可以在返回栏中看到游戏币各接口的请求结果。
![image](https://user-images.githubusercontent.com/82426537/114563092-ad887100-9ca1-11eb-9d2a-9fe0a0dbecaa.png)

## 实现原理

我们通过定制Postman的`Pre-request Script`，实现了请求的签名。

`Pre-request Script`脚本会在请求发送之前被执行。在脚本中，我们根据请求的方法、URL、请求参数、Body等信息，计算了centauri API v3的`sig`信息，并设置到了环境变量`{{sig}}`当中。而在请求发送时，环境变量中的`{{sig}}`将被替换成真实的签名值，实现了请求的签名。

关于Postman脚本的信息，可以参考[Pre-request Script](https://learning.getpostman.com/docs/postman/scripts/pre_request_scripts/)。

## 联系我们

如果你有任何疑问，欢迎邮件我们的centauri.public.test@gmail.com进行反馈。



