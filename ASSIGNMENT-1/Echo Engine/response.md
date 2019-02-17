
# Observations for echo_engine:

| #clients 	| Server Response Time(ns)                	|
|----------	|-----------------------------------------	|
|     5    	| 30,000 - 1,30,000 (majorly near 80,000)  	|
|     4    	| 30,000 - 1,20,000 (majorly near 80,000) 	|
|     3    	| 20,000 - 80,000 (majorly near 30,000 )  	|
|     2    	| 20,000 - 80,000 (majorly near 30,000 )  	|
|     1    	| 10,000 - 50,000 (majorly near 15,000)   	|


There wasn't any major difference, I think 5 clients is too less to overload the server and observe any significant difference in response time. The above table states a range of values(in nanosec) and also the most observed response time figure. Also there were some outliers (for example, some values for only client A running crossed the range of response time for 5 clients running at a time). This could have been possible due to some other processes running in the background.
