SELECT	t3.cno, c.cname, COUNT(e2.sno), num_s, min(t3.avg_mark1), max(t3.avg_mark1), t3.avg_mark2

FROM		(SELECT 	t1.cno, t1.term, t1.section, t1.avg_mark1, t2.avg_mark2, num_s

			 FROM		(SELECT			e.cno, e.term, e.section, AVG(e.mark) AS avg_mark1
						 FROM				enrollment e, class c, professor p
						 WHERE     	 	e.term < 'W12' AND e.cno = c.cno AND e.term = c.term AND e.section = c.section
						     				AND c.instructor = p.eid AND p.dept = 'Computer Science'
						 GROUP BY		e.cno, e.term, e.section) t1,
						 
						(SELECT		cno, term, section, AVG(mark) AS avg_mark2
						 FROM			enrollment
						 WHERE term < 'W12'
						 GROUP BY		cno, term) t2,
						 		 
						(SELECT tt.cno, tt.term, COUNT(*) AS num_s
						 FROM			   	 (SELECT		cno, term, section, AVG(mark) AS avg_mark1
						                    FROM			enrollment
					                       WHERE term < 'W12'
						 GROUP BY		cno, term, section) tt
          			GROUP BY tt.term, tt.cno) tt1					 
						 
			 WHERE	 t1.cno = t2.cno AND t1.term = t2.term AND tt1.term = t1.term AND tt1.cno = t1.cno) t3,
		   course c,
		   enrollment e2
WHERE   	c.cno = t3.cno AND e2.term = t3.term AND e2.cno = t3.cno
GROUP BY t3.cno
			;
