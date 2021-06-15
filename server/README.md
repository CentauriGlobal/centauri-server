# centauri
# centauri 服务端 游戏币类API  Postman脚本使用指南

## 简介

[Postman](https://www.getpostman.com/products)是一款业界知名的API开发工具。为了方便centauri接入业务快速上手，我们利用Postman强大的脚本扩展能力，编写了一套游戏币类API v3的脚本。通过这套脚本，业务可以在Postman上轻松的构造并发送游戏币类API请求。

## 导入

脚本和相关配置，统一保存在[Collection V2](https://go.pstmn.io/collection-v2)的`JSON`文件中。开发者可以点击Postman界面左上角的`Import`按钮或者通过`File`中的`Import`发起导入。选择本地的`centauri_server_v3.postman_collection.json`和`centauri_server_rsa.postman_collection.json`，点击确认后，导入便完成了。![wecom-temp-b5ab8f0d6ef44e43fc1a5bbb3c1e49f8](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-b5ab8f0d6ef44e43fc1a5bbb3c1e49f8.png)

你会发现在左侧Collection下新增了名为`centauri_server_v3`和`centauri_server_rsa`请求组。

## centauri_server_v3配置

#### 配置脚本

选中`centauri_server_v3`，右键`Edit`进入Collection的配置页面。如图所示。<img src="/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-e257078d385efe7e75997bb440cc5501.png" alt="wecom-temp-e257078d385efe7e75997bb440cc5501" style="zoom:100%;" />

在弹出的`Edit Collection`的浮层上部的多个分栏中，找到`Pre-request Scripts`一栏。

![wecom-temp-eada99ecc1beb00b8bb52733af64f19a](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-eada99ecc1beb00b8bb52733af64f19a.png)

其中红色方框为使用说明，需要配置的参数和请求的参数。

1.设置全局的appkey，这里的appkey是接入centauri，申请应用的时候生成的，可在centauri的接入界面找到（妥善保存）。

2.点击右上角的`Environment`和下方的`Edit`，设置`Global`参数APP_KEY，点击右上角的`save`按钮保存，如下所示：

![wecom-temp-24fe07be422b98d5d5583659e364c79e](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-24fe07be422b98d5d5583659e364c79e.png)

####  配置请求

现在回到请求的配置界面吧。接下来，你可以按照协议去设置请求的信息了，如方法、URL、请求参数、Body。(请求参数中的`{{ts}}`、`{{sig}}`为全局变量，请求是会自动填充，无需修改)

![image](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-1a78fb2352ada17e47f55b337c4cf6d7.png)

![wecom-temp-c3b8133db8c8d23e467ae1f1c3d740c8-3732533](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-c3b8133db8c8d23e467ae1f1c3d740c8-3732533.png)

####  发起请求

点击地址栏右侧的`Send`按钮，发送你的请求吧。

1.点击左下方的`console`，可以看到计算`V3签名`的各步骤的中间关键结果，以及最终算出的签名`sig`；

2.最终可以在返回栏中看到游戏币各接口的请求结果。
![wecom-temp-574dcd70d244d8b386374b98ccc2fb67](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-574dcd70d244d8b386374b98ccc2fb67.png)

## centauri_server_rsa配置

#### 配置脚本

选中`centauri_server_rsa`。在弹出的`Edit Collection`的浮层上部的多个分栏中，找到`Pre-request Scripts`一栏，如图所示。

![wecom-temp-5ced66b779b23b15ad19adc882a17a79](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-5ced66b779b23b15ad19adc882a17a79.png)

其中红色方框为使用说明，需要配置的参数和请求的参数。

1.设置全局的appkey，这里的appkey是接入centauri，申请应用的时候生成的，可在centauri的接入界面找到（妥善保存）。

1.设置全局的privatekey，这里的privatekey是需要业务使用OpenSSL生成私钥进行配置，公钥需要配置在centauri接入平台。

2.点击右上角的`Environment`和下方的`Edit`，设置`Global`参数`APP_KEY`和`PRIVATE_KEY`，点击右上角的`save`按钮保存，如下所示：

![wecom-temp-c7a4cf52d98ae18ddf0679e9abbb68d2-3732771](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-c7a4cf52d98ae18ddf0679e9abbb68d2-3732771.png)

脚本采用的`SHA256withRSA`算法使用`pmlib`工具库，需要先`install lib`，操作如下所示：

![wecom-temp-a4f0b43730ad8659b848d2d7a20c5293](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-a4f0b43730ad8659b848d2d7a20c5293.png)

###  配置请求

现在回到请求的配置界面吧。接下来，你可以按照协议去设置请求的信息了，如方法、URL、请求参数、Body。(请求参数中的`{{ts}}`、`{{sig}}`为全局变量，请求是会自动填充，无需修改)

![wecom-temp-65d3b5dac27d00a12704fd969d1d87ab](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-65d3b5dac27d00a12704fd969d1d87ab.png)

![wecom-temp-ff4760590a153cb395d3a37ad9664a3d-3732975](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-ff4760590a153cb395d3a37ad9664a3d-3732975.png)

###  发起请求

点击地址栏右侧的`Send`按钮，发送你的请求吧。

1.点击左下方的`console`，可以看到计算`rsa签名`的各步骤的中间关键结果，以及最终算出的签名`sig`；

2.最终可以在返回栏中看到游戏币各接口的请求结果。
![wecom-temp-ddd71983254d5b23416c964ca2112404](/Users/chrisqguo/Library/Application Support/typora-user-images/wecom-temp-ddd71983254d5b23416c964ca2112404.png)

## 实现原理

我们通过定制Postman的`Pre-request Script`，实现了请求的签名。

`Pre-request Script`脚本会在请求发送之前被执行。在脚本中，我们根据请求的方法、URL、请求参数、Body等信息，计算了centauri API 的`sig`信息，并设置到了环境变量`{{sig}}`当中。而在请求发送时，环境变量中的`{{sig}}`将被替换成真实的签名值，实现了请求的签名。

关于Postman脚本的信息，可以参考[Pre-request Script](https://learning.getpostman.com/docs/postman/scripts/pre_request_scripts/)。

## 联系我们

如果你有任何疑问，欢迎邮件我们的centauri.public.test@gmail.com进行反馈。



