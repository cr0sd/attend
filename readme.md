[![Build Status](https://travis-ci.com/cr0sd/attend.svg?token=WEvCwyPhUBEquCSrRa7q&branch=master)](https://travis-ci.com/cr0sd/attend)

### ATTEND

Create, manage sqlite3 database with following schema in the form of "sign in/out" entries:

`id,	name,	date,	time,	type`

Where `name` represents full name of attendee, `date` and `time` are distinct, and `type` indicates whether the entry is a "sign in" vs. a "sign out."
