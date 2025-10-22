#!/bin/bash
echo "Testing default site..."
curl -s http://localhost:8080/ | head -n 5

echo "Testing SPA fallback..."
curl -s http://localhost:8080/nonexistentpath | head -n 5

echo "Testing API..."
curl -s http://localhost:8080/api/hello

echo "Testing multi-site..."
curl -s -H "Host: site2.local" http://localhost:8080/ | head -n 5

