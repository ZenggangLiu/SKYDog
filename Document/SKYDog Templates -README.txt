
---------------------- HOW TO ----------------------------
1. 在 “~/Library/Developer/Xcode/” 下创建 “Templates/” 目录

2. 将SKYDog Templates.zip文件解压

3. 将所有文件复制到“~/Library/Developer/Xcode/Templates/”下

4. 在使用“New File”时，选择"==SKYDog=="区域的“Templates“：”C++ Header File”, “Metal File”...


====================================================================================
NOTE：在任意Template文件中我们可以使用如下 “Text Marcos” 定义：都使用三个"_"字符开始以及结尾
Text macros reference： https://help.apple.com/xcode/mac/11.4/#/dev7fe737ce0

___FILENAME___ : 带扩展名的文件名。例如：“RS_VertexLayout.hpp”

___FILEBASENAMEASIDENTIFIER___ : 无扩展名文件名(非法C标识符字符用“_”来替代)：得到的字符串只包括："_"，字母，数字。

___PRODUCTNAME___ : App名称

___PROJECTNAME___ : 当前项目名。例如：“ZiRender”

___USERNAME___ : 短Login名称。例如：“Bob”

___FULLUSERNAME___ : 长Login名称。例如：“Bob McCune”

___ORGANIZATIONNAME___ : The organization name defined in your Xcode project

___DATE___ : 日期。例如：“6/9/12”

___TIME___ : 时间。例如：“5:09 PM”

___YEAR___ : 四位年份。例如：“2024”

___UUID___ : UUID。例如：“D5C69A33-1414-47B7-875D-C4214DB5E417”
此处可以使用Modifier来得到多个UUID。例如： “___UUID:fileTag1___”， “___UUID:fileHash___”
