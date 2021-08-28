printf串口定向 不过不太好用
直接用sci的通讯协议 	UARTa_SendString函数
将数字转字符串可以用sprintf 不过很占资源，不建议使用
已经封装好数字转字符串的函数效率更高，但未优化。

*  @attention
 *  实验平台：F28335   核心板
 * 个人博客：http://yellownew.cn 