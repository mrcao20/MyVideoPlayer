# MyVideoPlayer
视频播放器初始模型
# 2018-9-17
视频播放器只有简单的播放功能，提供网络视频模型，并嵌入微型浏览器，实现腾讯视频的爬取(可以免广告)
# 2018-10-1
1.使用双线程实现直接播放和缓冲播放两种功能，并能实时切换。
  缓冲播放属于边下边播，播完即下完
2.与主程序建立Tcp连接，实时提交消息
3.output类增加输出枚举类型功能(暂未集成至主程序)    
