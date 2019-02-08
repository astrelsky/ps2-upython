
all:
	$(MAKE) -C base

clean:
	$(MAKE) -C base clean
	
fullclean:
	$(MAKE) -C base fullclean
	
remake: clean all

install:
	$(MAKE) -C base install