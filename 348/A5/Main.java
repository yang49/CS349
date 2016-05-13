import java.sql.*;
import java.util.*;
import java.lang.String;
import java.lang.Integer;

class Main{
	static final String JDBC_DRIVER = "com.ibm.db2.jcc.DB2Driver";
	static final String DB_URL = "jdbc:db2://linux.student.cs.uwaterloo.ca:50002/cs348";
	
	static final String USR = "db2guest";
	static final String PSW = "upKellynoisylair";
	
	//Main function
	public static void main(String[] args) {
		Connection conn = null;
		PreparedStatement stmt = null;
		ResultSet rs = null;
		//try
		try {
			//STEP 2: Register the JDBC driver
			Class.forName(JDBC_DRIVER);
		
			//STEP 3: Open a connection
			System.out.println("Connecting to database");
			conn = DriverManager.getConnection(DB_URL, USR, PSW);
			Statement s = conn.createStatement();
			s.executeUpdate("SET SCHEMA enrollment");
			s.close();
			System.out.println("Connected");
			Scanner sc = new Scanner(System.in);
			
			//While loop
			while(true) {
				////////////////////////////////////////////////////
				//Get the usr input
				System.out.println("Please enter the DEPARTMENT NAME: ");
				String Dept = sc.next();
				if (Dept.equals("exit")) break;
				System.out.println("The DEPARTMENT NAME -> " + Dept);
				
				System.out.println("Please enter the BEGINNING YEAR: ");
				String begin = sc.next();
				if (begin.equals("exit")) break;
				//System.out.println("The BEGINNING YEAR -> " + begin.substring(2,4));
				String yr_begin = "F" + begin.substring(2,4);
				
				System.out.println("Please enter the ENDDING YEAR: ");
				String end = sc.next();
				if (end.equals("exit")) break;
				String yr_end = "S" + end.substring(2,4);
				
				////////////////////////////////////////////////////
				
				//STEP 4: Excecute a query
				String sql;
				sql = "SELECT t.cno, t.term, t.mark1, t.mark2, t.mark3, t.num_s,c.cname, en2.num_e " +
						"FROM	(SELECT	t3.cno, t3.term, MAX(t3.avg_mark1) AS mark1, MIN(t3.avg_mark1) AS mark2, COUNT(t3.num_s) AS num_s, " +
						"					AVG(t3.avg_mark2) AS mark3 " +
						"		 FROM		(SELECT 	t1.cno, t1.term, t1.section, t1.avg_mark1, t2.avg_mark2, num_s " +
						"					 FROM		(SELECT 	temp.cno, temp.term, temp.section,  avg_mark1 " +
						"			 					 FROM		(SELECT			cno, term, section, AVG(mark) AS avg_mark1 " + 
						 "			 			 					 FROM				enrollment " +
						 "		    			 					 WHERE     	 	? < term AND term < ? " + // 1 2
						 "		    			 					 GROUP BY		cno, term, section) temp, " +
						 "        								 class c, professor p " + 
						 "								 WHERE  	temp.cno = c.cno AND temp.term = c.term AND temp.section = c.section " + 
						 "											AND c.instructor = p.eid AND p.dept = ? " + 	// 3
						 "							    ORDER BY temp.term) t1, " + 
						 
						 								// Calculate the average mark for every class
						 "								(SELECT		cno, term, AVG(mark) AS avg_mark2 " + 
						 "			 			 		 FROM			enrollment " + 
						 "			 					 WHERE ? < term AND term < ? " + 	//4 5
						 "			 					 GROUP BY		cno, term) t2, " + 
						 
						 								// Count how many sections for a course, Since in ISQL "GROUP BY"
						 								// is different from MYSQL
						 "					   		(SELECT tt.cno, tt.term, COUNT(*) AS num_s " + 
						 "			 					 FROM	  	 (SELECT		cno, term, section, AVG(mark) AS avg_mark1 " +
						 "					  	  			  		  FROM			enrollment " + 
						 "						  			  		  WHERE ? < term AND term < ? " + // 6 7 
						 "						  			  		  GROUP BY		cno, term, section) tt " + 
						 "			 			 		 GROUP BY tt.term, tt.cno) tt1 " + 
						 
						 "			WHERE	 t1.cno = t2.cno AND t1.term = t2.term AND tt1.term = t1.term AND tt1.cno = t1.cno) t3 " +
						 "			GROUP BY t3.cno, t3.term )t, " + 
						 
						 		// Count how many students enroll in a certain class
						 "		(SELECT		COUNT(sno) AS num_e, cno, term " +
						 "		 FROM			enrollment " +
						 "		 GROUP BY	cno, term) en2, " +
						 "		course c " +
						 "WHERE	c.cno = t.cno AND t.cno = en2.cno AND t.term = en2.term " +
						 "ORDER BY t.term ";
				// inserting the input data
				stmt = conn.prepareStatement(sql);
				stmt.setString(1, yr_begin);
				stmt.setString(2, yr_end);
				stmt.setString(3, Dept);
				stmt.setString(4, yr_begin);
				stmt.setString(5, yr_end);
				stmt.setString(6, yr_begin);
				stmt.setString(7, yr_end);
				
				//execute the query
				rs = stmt.executeQuery();
				
				System.out.println("Year " + begin);

				System.out.printf("%-6s %-18s %-12s %-12s %-12s %-12s %s\n","C#","NAME","Enrollment", "#Section", 	
																								"CourseAve", "MaxClassAve", "MinClassAve");
				//Printing the result of the query
				while(rs.next()) {
					String cno = rs.getString("cno");
					String cname = rs.getString("cname");
					String term = rs.getString("term");
					String avg_mark1 = rs.getString("mark1");
					String avg_mark2 = rs.getString("mark2");
					String avg_mark3 = rs.getString("mark3");
					String num_e = rs.getString("num_e");
					String num_s = rs.getString("num_s");
					System.out.printf("%-6s %.17s    %-12s %-12s %-12s %-12s %s\n",cno,cname,num_e, num_s, 
																							 avg_mark3,avg_mark1 ,avg_mark2);
				}
				
				System.out.println( "Year " + end );
			}	// end while
				rs.close();
				stmt.close();
		}catch(SQLException se) {
			se.printStackTrace();
		}catch(Exception e) {
			e.printStackTrace();
		}// end try
	}//end main function
	
} //end with Main class