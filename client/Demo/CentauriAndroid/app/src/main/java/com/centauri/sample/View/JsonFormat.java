package com.centauri.sample.View;

/**
 * Created by zachzeng on 2017/1/11.
 */

public class JsonFormat {

    public static String format(String data){
        if(null ==  data || "".equals(data)){
            return "";
        }

        StringBuilder result = new StringBuilder();
        char last = '\0';
        char current = '\0';
        int index = 0;
        int length = data.length();

        for(int i = 0;i<length;i++){
            last = current;
            current = data.charAt(i);
            switch (current){
                //遇到{[换行，且下一行缩进
                case '{':
                case '[':
                    result.append(current);
                    result.append('\n');
                    index++;    //统计缩进次数
                    addBlank(result,index);
                    break;
                //遇到}]换行，且当前行缩进
                case '}':
                case ']':
                    result.append('\n');
                    index--;
                    addBlank(result,index);
                    result.append(current);
                    break;
                //遇到,换行;注释不换行
                case ',':
                    result.append(current);
                    if(last != '\\'){
                        result.append('\n');
                        addBlank(result,index);
                    }
                    break;
                default:
                    result.append(current);
            }
        }
        return result.toString();
    }

    private static void addBlank(StringBuilder sb, int count){
        for(int i=0;i<count;i++){
            sb.append('\t');
        }
    }
}
