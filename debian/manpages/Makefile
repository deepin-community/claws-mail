#
# generate manpages from POD input
#

CENTER := 'Claws Mail'
MANPAGES := $(shell ls -1 *.pod | sort | awk -F. '{print "B<"$$1">(7),"}' | tr '\n' "\\n" )
POD1 := matcherrc2perlfilter.pod
POD7 := $(filter-out $(POD1),$(wildcard *.pod))
PODIN := $(wildcard *.pod.in)
GPODS := $(PODIN:%.pod.in=%.pod)
PAGES1 := $(POD1:%.pod=%.1)
PAGES7 := $(POD7:%.pod=%.7) $(GPODS:%.pod=%.7)
P2M1 := pod2man -s 1 -u -c $(CENTER) -r $(VERSION) -d $(DATE)
P2M7 := pod2man -s 7 -u -c $(CENTER) -r $(VERSION) -d $(DATE)

all: $(PAGES1) $(PAGES7)

%.pod: %.pod.in
	@sed "s/MANPAGES/$(MANPAGES)/" < $< > $@

%.1: %.pod
	$(P2M1) $< > $@

%.7: %.pod
	$(P2M7) $< > $@

clean:
	rm -f $(PAGES1) $(PAGES7) $(GPODS)

.PHONY: all clean
