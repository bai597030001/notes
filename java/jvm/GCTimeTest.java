import java.util.HashMap;
 
 
public class GCTimeTest {
 
 static HashMap map = new HashMap();
  
	public static void main(String[] args){
		long begintime = System.currentTimeMillis();
		while (true) {
			if(map.size()*512/1024/1024>=400){
				map.clear();
				System.out.println("clean map(map size: " + map.size() + ")");
			}
			byte[] b1;
			for(int j=0;j<100;j++){
				b1 = new byte[512];
				map.put(System.nanoTime(), b1);
			}
		}
	}
}