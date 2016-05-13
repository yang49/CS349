SELECT tt.cno, tt.term, COUNT(*)		
FROM			   	 (SELECT		cno, term, section, AVG(mark) AS avg_mark1
						  FROM			enrollment
						  WHERE term LIKE '_12'
						  GROUP BY		cno, term, section) tt
GROUP BY tt.term, tt.cno
						 ;
